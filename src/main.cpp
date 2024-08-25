#include <scene.hpp>
#include <camera.hpp>
#include <material.hpp>
#include <lenses/pinhole.hpp>
#include <image_buffers/hdr_buffer.hpp>
#include <integrators/integrator.hpp>
#include <integrators/ray_caster.hpp>
#include <integrators/path_tracer.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>

pt::scene create_scene()
{
    pt::scene s;
    material red_mat { {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    material ground_mat { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
    material light_mat { {1.0f, 1.0f, 1.0f}, {10.0f, 10.0f, 5.0f}};

    auto const red_ID = s.add_material(red_mat);
    auto const ground_ID = s.add_material(ground_mat);
    auto const light_ID = s.add_material(light_mat);

    s.add_shape(pt::shapes::sphere{1.0f, red_ID}, glm::identity<glm::mat4>());

    s.add_shape(pt::shapes::sphere{10'000.0f, ground_ID},
                glm::translate(glm::identity<glm::mat4>(), {0.0f, -10'001.0f, 0.0f}));

    s.add_shape(pt::shapes::sphere{0.5f, light_ID},
                glm::translate(glm::identity<glm::mat4>(), {-1.75f, 1.5f, 0.85f}));

    return s;
}

auto create_camera()
{
    return pt::camera(pt::lenses::pinhole());
}

auto create_integrator()
{
    // return pt::integrator<pt::integrators::ray_caster>();
    return pt::integrator<pt::integrators::path_tracer>();
}

void save_image(std::filesystem::path file_path, pt::image_buffers::hdr_buffer const& image_buffer)
{
    std::ofstream image_file(file_path);

    auto const image_width = image_buffer.get_width();
    auto const image_height = image_buffer.get_height();

    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    auto const& data = image_buffer.get_data();

    for (int j = image_height - 1; j >= 0; --j) 
    {
        for (int i = 0; i < image_width; ++i)
        {
            auto const& color = data[i][image_height - j];

            int R = color.r * 255;
            int G = color.g * 255;
            int B = color.b * 255;

            if (R > 255) R = 255;
            if (R < 0) R = 0;

            if (G > 255) G = 255;
            if (G < 0) G = 0;

            if (B > 255) B = 255;
            if (B < 0) B = 0;

            image_file << R << ' ' << G << ' ' << B << '\n';
        }
    }
}

int main(int argc, char** argv)
{
    auto const sc = create_scene();
    auto const cam = create_camera();
    auto integrator = create_integrator();

    auto const image_buffer = integrator.render_scene(sc, cam);

    save_image("output.ppm", image_buffer);

    return 0;
}