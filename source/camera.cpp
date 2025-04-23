#include "../header/camera.h"
#include "../header/constants.h"
#include "../header/util.h"
#include <chrono>
#include <fstream>
#include <iostream>

Camera::Camera(float positionX, float positionY, float positionZ, float lookatX, float lookatY, float lookatZ,
    float upX, float upY, float upZ)
{
    m_position = Point(positionX, positionY, positionZ);
    m_lookat = Point(lookatX, lookatY, lookatZ);
    m_up = Vector(upX, upY, upZ);
}

void Camera::render(World* world)
{
    std::ofstream outfile;

    // Attempt to open outfile
    outfile.open("output/image.ppm");

    // Verify file is open
    if (outfile.is_open()) {

        // Set up PPM file
        outfile << "P3" << std::endl;
        outfile << IMAGE_WIDTH << " " << IMAGE_HEIGHT << std::endl;
        outfile << "255" << std::endl;

        // First convert objects from world coordinates to camera coordinates
        Vector n(m_position.getPoint() - m_lookat.getPoint());
        n.normalize();
        Vector u = m_up.cross(&n);
        u.normalize();
        Vector v = n.cross(&u);

        // Create view transform matrix
        Eigen::Matrix4d viewTransform;
        viewTransform << u.getVector()(0), u.getVector()(1), u.getVector()(2),
            (m_position.getPoint() * -1).dot(u.getVector()), v.getVector()(0), v.getVector()(1), v.getVector()(2),
            (m_position.getPoint() * -1).dot(v.getVector()), n.getVector()(0), n.getVector()(1), n.getVector()(2),
            (m_position.getPoint() * -1).dot(n.getVector()), 0, 0, 0, 1;

        world->transformAllObjects(viewTransform);
        world->transformLightSources(viewTransform);

        if (USE_KD_TREES) {
            auto start = std::chrono::steady_clock::now();
            world->buildKDTree();
            auto finish = std::chrono::steady_clock::now();
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
            std::cout << "Building tree took: " << elapsed_seconds << " seconds." << std::endl;
        }

        // world location of top-left pixel of film plane is (-w/2, h/2, f)
        // where w = width of film plane, h = height of film plane, f = focal length
        // to shoot a ray through the center of this pixel, we shift right and down
        // half a pixel, where each pixel is (h/H) tall and (w/W) wide.
        // H and W represent the height and width of the image in pixels
        float pixelWidth = FILM_PLANE_WIDTH / (IMAGE_WIDTH * 1.0);
        float pixelHeight = FILM_PLANE_HEIGHT / (IMAGE_HEIGHT * 1.0);

        Eigen::Vector3d topLeftRay(
            -1 * FILM_PLANE_WIDTH / 2.0 + pixelWidth / 2.0, FILM_PLANE_HEIGHT / 2.0 - pixelHeight / 2.0, -FOCAL_LENGTH);

        auto start = std::chrono::steady_clock::now();

        std::vector<Radiance> radianceMap;
        float logSum = 0.0f;
        float maxIlluminance = 0.0f;
        // Loop through the film plane and populate radiance map
        for (int i = 0; i < IMAGE_HEIGHT; i++) {
            for (int j = 0; j < IMAGE_WIDTH; j++) {

                // Supersampling shoots 4 different rays through each pixel of the film plane
                // These 4 rays are designed to be random in each of the four quadrants of the pixel
                if (SUPERSAMPLING) {
                    float pixelWidthOver4 = pixelWidth / 4.0;
                    float pixelHeightOver4 = pixelHeight / 4.0;

                    // Top Left quadrant
                    Ray* ray1
                        = new Ray(0, 0, 0, topLeftRay(0) + j * pixelWidth - randomBetweenInclusive(pixelWidthOver4),
                            topLeftRay(1) - i * pixelHeight + randomBetweenInclusive(pixelHeightOver4), topLeftRay(2));

                    // Top Right quadrant
                    Ray* ray2
                        = new Ray(0, 0, 0, topLeftRay(0) + j * pixelWidth + randomBetweenInclusive(pixelWidthOver4),
                            topLeftRay(1) - i * pixelHeight + randomBetweenInclusive(pixelHeightOver4), topLeftRay(2));

                    // Bottom Left quadrant
                    Ray* ray3
                        = new Ray(0, 0, 0, topLeftRay(0) + j * pixelWidth - randomBetweenInclusive(pixelWidthOver4),
                            topLeftRay(1) - i * pixelHeight - randomBetweenInclusive(pixelHeightOver4), topLeftRay(2));

                    // Bottom Right quadrant
                    Ray* ray4
                        = new Ray(0, 0, 0, topLeftRay(0) + j * pixelWidth + randomBetweenInclusive(pixelWidthOver4),
                            topLeftRay(1) - i * pixelHeight - randomBetweenInclusive(pixelHeightOver4), topLeftRay(2));

                    Radiance totalRadiance = world->spawnRay(ray1);
                    totalRadiance.addRadiance(world->spawnRay(ray2));
                    totalRadiance.addRadiance(world->spawnRay(ray3));
                    totalRadiance.addRadiance(world->spawnRay(ray4));

                    // Averaging the radiances
                    totalRadiance.scaleRadiance(0.25);
                    radianceMap.push_back(totalRadiance);

                    float illuminance = totalRadiance.getIlluminance();
                    if (illuminance > maxIlluminance) {
                        maxIlluminance = illuminance;
                    }

                    logSum += std::log(illuminance + 0.0001f);

                    delete ray1;
                    delete ray2;
                    delete ray3;
                    delete ray4;

                } else {
                    // 0, 0, 0 since we are now in camera coordinates
                    Ray* ray = new Ray(
                        0, 0, 0, topLeftRay(0) + j * pixelWidth, topLeftRay(1) - i * pixelHeight, topLeftRay(2));
                    Radiance radiance = world->spawnRay(ray);
                    radianceMap.push_back(radiance);

                    float illuminance = radiance.getIlluminance();
                    if (illuminance > maxIlluminance) {
                        maxIlluminance = illuminance;
                    }

                    logSum += std::log(illuminance + 0.0001f);

                    delete ray;
                }
            }
        }

        float logAverageIlluminance = std::exp(logSum / (IMAGE_HEIGHT * IMAGE_WIDTH));

        // Ward TR
        if (WHICH_TONE_REPRODUCTION == 0) {
            float scaleFactorNumerator = 1.219f + std::pow(maxIlluminance / 2.0f, 0.4f);
            float scaleFactorDenominator = 1.219f + std::pow(logAverageIlluminance, 0.4f);
            float scaleFactor = std::pow(scaleFactorNumerator / scaleFactorDenominator, 2.5f);
            // Tone Reproduction
            for (int i = 0; i < IMAGE_HEIGHT; i++) {
                for (int j = 0; j < IMAGE_WIDTH; j++) {
                    Radiance radianceTarget = radianceMap.at(i * IMAGE_WIDTH + j);
                    radianceTarget.scaleRadiance(scaleFactor);
                    radianceTarget.capRadiance();
                    Color c = Color(radianceTarget);
                    outfile << c.getRed255() << " " << c.getGreen255() << " " << c.getBlue255() << " ";
                }
                outfile << std::endl;
            }
        } else {
            // Reinhard TR
            // Key value will be log average luminance

            float scaleFactor;
            if (REINHARD_USE_LOG_AVERAGE) {
                scaleFactor = 0.18f / logAverageIlluminance;
            } else if (REINHARD_USE_USER_CONSTANT) {
                scaleFactor = 0.18f / REINHARD_USER_CONSTANT;
            } else if (REINHARD_KEY_VALUE_PIXEL_X >= 0 && REINHARD_KEY_VALUE_PIXEL_X < IMAGE_WIDTH
                && REINHARD_KEY_VALUE_PIXEL_Y >= 0 && REINHARD_KEY_VALUE_PIXEL_Y < IMAGE_HEIGHT) {
                scaleFactor = 0.18f
                    / radianceMap.at(REINHARD_KEY_VALUE_PIXEL_Y * IMAGE_WIDTH + REINHARD_KEY_VALUE_PIXEL_X)
                          .getIlluminance();
            } else {
                // Default to log average
                scaleFactor = 0.18f / logAverageIlluminance;
            }

            for (int i = 0; i < IMAGE_HEIGHT; i++) {
                for (int j = 0; j < IMAGE_WIDTH; j++) {
                    Radiance radianceTarget = radianceMap.at(i * IMAGE_WIDTH + j);

                    radianceTarget.scaleRadiance(scaleFactor);
                    float redReflectance = radianceTarget.getRadianceRed() / (1.0f + radianceTarget.getRadianceRed());
                    float greenReflectance
                        = radianceTarget.getRadianceGreen() / (1.0f + radianceTarget.getRadianceGreen());
                    float blueReflectance
                        = radianceTarget.getRadianceBlue() / (1.0f + radianceTarget.getRadianceBlue());

                    Radiance radianceFinal = Radiance(redReflectance * maxIlluminance,
                        greenReflectance * maxIlluminance, blueReflectance * maxIlluminance);
                    radianceFinal.capRadiance();

                    Color c = Color(radianceFinal);
                    outfile << c.getRed255() << " " << c.getGreen255() << " " << c.getBlue255() << " ";
                }
                outfile << std::endl;
            }
        }

        auto finish = std::chrono::steady_clock::now();
        double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
        std::cout << "Ray Tracing Took : " << elapsed_seconds << " seconds." << std::endl;

        // Need to verify that intersection happens at a point past the film plane - TODO

        outfile.close();
        delete world;
    } else {
        std::cerr << "COULD NOT OPEN FILE" << std::endl;
    }
}