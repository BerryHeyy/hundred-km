#pragma once

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

namespace hkm
{
    struct GraphicsPipelineConfigInfo
    {
        VkViewport viewport;
        VkRect2D scissor;
        VkRenderPass render_pass = nullptr;
    };

    class GraphicsPipeline
    {
    public:
        const std::string vert_name, frag_name;
        const VkDevice device;

        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline;

        GraphicsPipeline(
            const VkDevice& device,
            const std::string& vert_name,
            const std::string& frag_name,
            const GraphicsPipelineConfigInfo& config_info
            );
        ~GraphicsPipeline();

    private:
        const GraphicsPipelineConfigInfo config_info;

        void create_graphics_pipeline();

        VkShaderModule create_shader_module(const std::vector<char>& code); 

    };
}
