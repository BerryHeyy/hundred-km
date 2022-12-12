#pragma once

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

#include "logical_device.hpp"

namespace hkm
{
    struct GraphicsPipelineConfigInfo
    {
        VkRenderPass render_pass = nullptr;
    };

    class GraphicsPipeline
    {
    public:
        GraphicsPipeline();

        GraphicsPipeline(
            const LogicalDevice* device,
            const std::string& vert_name,
            const std::string& frag_name,
            const GraphicsPipelineConfigInfo& config_info
            );

        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline(GraphicsPipeline&&) = delete;
        
        ~GraphicsPipeline();

        GraphicsPipeline& operator = (const GraphicsPipeline&) = delete;
        GraphicsPipeline& operator = (GraphicsPipeline&& source);

        VkPipeline get_graphics_pipeline_handle() const;

    private:
        std::string vert_name = "", frag_name = "";
        const LogicalDevice* device;

        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline_handle;

        GraphicsPipelineConfigInfo config_info {};

        void create_graphics_pipeline();

        VkShaderModule create_shader_module(const std::vector<char>& code) const; 

    };
}
