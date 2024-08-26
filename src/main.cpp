#include <scene.hpp>
#include <camera.hpp>
#include <material.hpp>
#include <lenses/pinhole.hpp>
#include <image_buffers/hdr_buffer.hpp>
#include <image_buffers/rgb_buffer.hpp>
#include <post_processes/gamma_correction.hpp>
#include <post_processes/clamp_to_rgb.hpp>
#include <post_processes/modified_reinhard.hpp>
#include <integrators/integrator.hpp>
#include <integrators/ray_caster.hpp>
#include <integrators/path_tracer.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>
#include <chrono>
#include <iostream>

pt::scene create_scene()
{
    pt::scene s;
    material red_mat { {0.545f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    material green_mat { {86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f}, {0.0f, 0.0f, 0.0f}};
    material white_mat { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};

    // auto const scale = 60.0f;
    // material light_mat { {1.0f, 1.0f, 1.0f}, scale * glm::vec3{1.0f, 214.0f / 255.0f, 170.0f / 255.0f}};

    auto const scale = 1.0f;
    material light_mat { {1.0f, 1.0f, 1.0f}, scale * glm::vec3{1.0f, 1.0f, 1.0f}};

    auto const small_scale = 2.5f;
    material small_light_mat {{ 1.0f, 1.0f, 1.0f }, small_scale * glm::vec3{1.0f, 0.75f, 0.5f}};

    auto const red_ID = s.add_material(red_mat);
    auto const green_ID = s.add_material(green_mat);
    auto const white_ID = s.add_material(white_mat);
    
    auto const light_ID = s.add_material(light_mat);
    auto const small_light_ID = s.add_material(small_light_mat);

    // add sphere
    s.add_shape(pt::shapes::sphere{0.3f, white_ID},
                glm::translate(glm::identity<glm::mat4>(), {-0.3f, 0.3f, -0.3f}));

    // add right box
    s.add_shape(pt::shapes::box(0.4f, 0.4f, 0.4f, white_ID),
                glm::rotate(glm::translate(glm::identity<glm::mat4>(), {0.25f, 0.2f, 0.2f}),
                glm::radians(-20.0f), {0.0f, 1.0f, 0.0f}));

    // add rear wall
    s.add_shape(pt::shapes::plane{10.0f, 10.0f, white_ID},
                glm::translate(glm::identity<glm::mat4>(), {0.0f, 0.0f, -0.75f}));

    // add left wall
    s.add_shape(pt::shapes::plane{10.0f, 10.0f, red_ID},
                glm::rotate(glm::translate(glm::identity<glm::mat4>(), {-0.75f, 0.0f, 0.0f}),
                glm::radians(90.0f), { 0.0f, 1.0f, 0.0f }));

    // add right wall
    s.add_shape(pt::shapes::plane{10.0f, 10.0f, green_ID},
                glm::rotate(glm::translate(glm::identity<glm::mat4>(), {0.75f, 0.0f, 0.0f}),
                glm::radians(-90.0f), { 0.0f, 1.0f, 0.0f }));

    // add floor
    s.add_shape(pt::shapes::plane{10.0f, 10.0f, white_ID},
                glm::rotate(glm::translate(glm::identity<glm::mat4>(), {0.0f, 0.0f, 0.0f}),
                glm::radians(-90.0f), { 1.0f, 0.0f, 0.0f }));

    // add ceiling
    // s.add_shape(pt::shapes::plane{10.0f, 10.0f, white_ID},
    //             glm::rotate(glm::translate(glm::identity<glm::mat4>(), {0.0f, 1.3f, 0.0f}),
    //             glm::radians(90.0f), { 1.0f, 0.0f, 0.0f }));

    // add light
    // s.add_shape(pt::shapes::plane{0.25f, 0.25f, light_ID},
    //             glm::rotate(glm::translate(glm::identity<glm::mat4>(), {0.0f, 1.299f, 0.0f}),
    //             glm::radians(90.0f), { 1.0f, 0.0f, 0.0f }));

    // add skylight
    s.add_shape(pt::shapes::plane{1.5f, 1.5f, light_ID},
                glm::rotate(glm::translate(glm::identity<glm::mat4>(), {0.0f, 1.3f, 0.0f}),
                glm::radians(90.0f), { 1.0f, 0.0f, 0.0f }));

    // add small light
    s.add_shape(pt::shapes::sphere{0.05f, small_light_ID},
                glm::translate(glm::identity<glm::mat4>(), {0.2f, 0.45f, 0.2f}));

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

void save_image(std::filesystem::path file_path, pt::image_buffers::rgb_buffer const& image_buffer)
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

            image_file << color.r << ' ' << color.g << ' ' << color.b << '\n';
        }
    }
}

void render()
{
    auto const sc = create_scene();
    auto const cam = create_camera();
    auto integrator = create_integrator();

    auto render_props = decltype(integrator)::render_properties{};
    render_props.num_samples = 512u;
    render_props.resolution = { 1920u, 1080u };

    auto const start = std::chrono::high_resolution_clock::now();
    auto const raw_image_buffer = integrator.render_scene(sc, cam, render_props);
    auto const end = std::chrono::high_resolution_clock::now();

    auto const time = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "Render time: " << std::to_string(time.count()) << " s." << std::endl;

    pt::post_processes::clamp_to_rgb clamp;
    save_image("raw_output.ppm", clamp(raw_image_buffer));

    pt::post_processes::gamma_correction gamma;
    save_image("gamma_corrected.ppm", clamp(gamma(raw_image_buffer)));

    pt::post_processes::modified_reinhard tone_map;
    save_image("tone_mapped.ppm", clamp(gamma(tone_map(raw_image_buffer))));
}

int main(int argc, char** argv)
{
    render();

    return 0;
}