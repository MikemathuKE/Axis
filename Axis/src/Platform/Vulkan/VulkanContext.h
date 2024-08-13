#pragma once

#include "Axis/Renderer/GraphicsContext.h"

struct GLFWwindow;

//struct VkSurfaceCapabilitiesKHR;
//struct VkSurfaceFormatKHR;
//struct VkPresentModeKHR;
//struct VkDebugUtilsMessengerCreateInfoEXT;
//struct VkPhysicalDevice;
//struct VkInstance;
//struct VkDebugUtilsMessengerEXT;
//struct VkSurfaceKHR;

#include <vulkan/vulkan.h>

namespace Axis
{

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		// Can add transfer family if for transfer queue

		bool isComplete() {
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
		void CreateSurface();
		void PickPhysicalDevice();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		

	private:
		GLFWwindow *m_WindowHandle;
		VkInstance m_VulkanInstance = NULL;
		VkDebugUtilsMessengerEXT m_DebugMessenger = NULL;
		VkSurfaceKHR m_WindowSurface = NULL;
		VkPhysicalDevice m_PhysicalDevice = NULL;
	};

}