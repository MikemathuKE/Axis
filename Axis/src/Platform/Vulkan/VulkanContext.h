#pragma once

#include "Axis/Renderer/GraphicsContext.h"

struct GLFWwindow;

#include <vulkan/vulkan.h>

namespace Axis
{

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		// Can add transfer family if for transfer queue

		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow *windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		void CreateVulkanInstance();
		void SetupDebugMessenger();
		void CreateWindowSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateDescriptorSetLayout();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		VkFormat FindDepthFormat();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		

	private:
		GLFWwindow *m_WindowHandle;
		VkInstance m_VulkanInstance = NULL;
		VkDebugUtilsMessengerEXT m_DebugMessenger = NULL;
		VkSurfaceKHR m_WindowSurface = NULL;
		VkPhysicalDevice m_PhysicalDevice = NULL;
		VkDevice m_Device = NULL;
		VkQueue m_GraphicsQueue = NULL;
		VkQueue m_PresentQueue = NULL;
		VkSwapchainKHR m_SwapChain = NULL;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;
		VkRenderPass m_RenderPass;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};

}