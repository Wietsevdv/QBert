#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"


#include "../3rdParty/imgui-1.89.4/imgui.h"
#include "../3rdParty/imgui-1.89.4/backends/imgui_impl_sdl2.h"
#include "../3rdParty/imgui-1.89.4/backends/imgui_impl_opengl2.h"
#include "../3rdParty/imgui-1.89.4/include/imgui_plot.h"

//imgui variables begin
#include <chrono>
#include <vector>
void TrashTheCache();
void TrashTheCacheObj();
void TrashTheCacheObjAlt();
bool g_CacheTrashed{ false };
bool g_CacheTrashedObj{ false };
bool g_CacheTrashedObjAlt{ false };
std::vector<float> g_Timings{};
std::vector<float> g_TimingsObj{};
std::vector<float> g_TimingsObjAlt{};

struct Transform
{
	float matrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
};

class GameObject3D
{
public:
	Transform transform;
	int ID = 1;
};

class GameObject3DAlt
{
public:
	Transform* transform;
	int ID = 1;
};
//imgui variables end

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	//ImGui_ImplOpenGL2_NewFrame();
	//ImGui_ImplSDL2_NewFrame(m_window);
	//ImGui::NewFrame();

	//ImU32 plotColorRed = ImColor(255, 0, 0);
	//ImU32 plotColorGreen = ImColor(0, 255, 0);
	//ImU32 plotColorBlue = ImColor(0, 0, 255);
	//uint32_t selectionStart{ 0 }, selectionLength{ 0 };

	////EXERCISE 1
	//ImGui::Begin("Exercise 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	//if (ImGui::Button("TrashTheCaching"))
	//{
	//	TrashTheCache();
	//};
	//
	//if (g_CacheTrashed)
	//{
	//	//DATA
	//	const int dataCount{ static_cast<int>(g_Timings.size()) };
	//	float* pX_data = new float[dataCount] {1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f};
	//	float* pY_data = new float[dataCount] {};
	//	for (int i{}; i < static_cast<int>(g_Timings.size()); ++i)
	//	{
	//		pY_data[i] = g_Timings[i];
	//	}

	//	ImGui::SetWindowSize(ImVec2{ 600, 300 });
	//	ImGui::PlotConfig conf;
	//	conf.values.xs = pX_data;
	//	conf.values.count = dataCount;
	//	conf.values.ys = pY_data;
	//	conf.values.color = plotColorRed;
	//	conf.scale.min = 0.f;
	//	conf.scale.max = 6.5f;
	//	conf.tooltip.show = true;
	//	conf.grid_x.show = false;
	//	conf.grid_x.size = 1.f;
	//	conf.grid_x.subticks = 1;
	//	conf.grid_y.show = true;
	//	conf.grid_y.size = 1.f;
	//	conf.grid_y.subticks = 1;
	//	conf.selection.show = false;
	//	conf.selection.start = &selectionStart;
	//	conf.selection.length = &selectionLength;
	//	conf.frame_size = ImVec2(150, 100);
	//	ImGui::Plot("plot1", conf);

	//	ImGui::End();

	//	delete[] pX_data;
	//	delete[] pY_data;
	//}
	//ImGui::End();

	////EXERCISE 2
	//ImGui::Begin("Exercise 2", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	//if (ImGui::Button("TrashTheCache GameObj3D"))
	//{
	//	TrashTheCacheObj();
	//};

	//if (g_CacheTrashedObj)
	//{
	//	//DATA
	//	const int dataCount{ static_cast<int>(g_TimingsObj.size()) };
	//	float* pX_data = new float[dataCount] {1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f};
	//	float* pY_data = new float[dataCount] {};
	//	for (int i{}; i < static_cast<int>(g_TimingsObj.size()); ++i)
	//	{
	//		pY_data[i] = g_TimingsObj[i];
	//	}

	//	ImGui::SetWindowSize(ImVec2{ 600, 300 });
	//	ImGui::PlotConfig conf;
	//	conf.values.xs = pX_data;
	//	conf.values.count = dataCount;
	//	conf.values.ys = pY_data;
	//	conf.values.color = plotColorGreen;
	//	conf.scale.min = 0.f;
	//	conf.scale.max = 55.f;
	//	conf.tooltip.show = true;
	//	conf.grid_x.show = false;
	//	conf.grid_x.size = 1.f;
	//	conf.grid_x.subticks = 1;
	//	conf.grid_y.show = true;
	//	conf.grid_y.size = 1.f;
	//	conf.grid_y.subticks = 1;
	//	conf.selection.show = false;
	//	conf.selection.start = &selectionStart;
	//	conf.selection.length = &selectionLength;
	//	conf.frame_size = ImVec2(150, 100);
	//	ImGui::Plot("plot1", conf);

	//	if (ImGui::Button("TrashTheCache GameObj3DAlt"))
	//	{
	//		TrashTheCacheObjAlt();
	//	};

	//	if (g_CacheTrashedObjAlt)
	//	{
	//		//DATA
	//		const int dataCountAlt{ static_cast<int>(g_TimingsObjAlt.size()) };
	//		float* pY_dataAlt = new float[dataCountAlt] {};
	//		for (int i{}; i < static_cast<int>(g_TimingsObjAlt.size()); ++i)
	//		{
	//			pY_dataAlt[i] = g_TimingsObjAlt[i];
	//		}

	//		conf.values.ys = pY_dataAlt;
	//		conf.scale.max = 20.f;
	//		conf.values.color = plotColorBlue;

	//		ImGui::Plot("plot2", conf);

	//		//COMBINED
	//		const float* y_data[2] = { pY_data, pY_dataAlt };
	//		ImU32 colors[2] = { plotColorGreen, plotColorBlue };

	//		ImGui::PlotConfig confCombined;
	//		confCombined.values.xs = pX_data;
	//		confCombined.values.count = dataCountAlt;
	//		confCombined.values.ys_list = y_data;
	//		confCombined.values.ys_count = 2;
	//		confCombined.values.colors = colors;
	//		confCombined.scale.min = 0.f;
	//		confCombined.scale.max = 55.f;
	//		confCombined.tooltip.show = true;
	//		confCombined.grid_x.show = false;
	//		confCombined.grid_x.size = 1.f;
	//		confCombined.grid_x.subticks = 1;
	//		confCombined.grid_y.show = true;
	//		confCombined.grid_y.size = 1.f;
	//		confCombined.grid_y.subticks = 1;
	//		confCombined.selection.show = false;
	//		confCombined.selection.start = &selectionStart;
	//		confCombined.selection.length = &selectionLength;
	//		confCombined.frame_size = ImVec2(150, 100);

	//		ImGui::Text("Combined:");
	//		ImGui::Plot("plot3", confCombined);

	//		delete[] pY_dataAlt;
	//	}

	//	delete[] pX_data;
	//	delete[] pY_data;
	//}
	//ImGui::End();

	//ImGui::Render();
	//ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

void TrashTheCache()
{
	g_Timings.clear();

	const int arraySize{ 10000000 };
	int* pBigArray = new int[arraySize] { 1 };
	std::vector<long long> elapsedTimes{};
	float averageTime{};
	const int nrOfSamples{ 15 };

	for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
	{
		//Timings
		for (int j{}; j < nrOfSamples; ++j)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			for (int i{}; i < arraySize; i += stepSize)
			{
				pBigArray[i] *= 2;
			}

			auto endTime = std::chrono::high_resolution_clock::now();
			long long elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
			elapsedTimes.push_back(elapsedTime);

			//values to 1
			for (int i{}; i < arraySize; ++i)
			{
				pBigArray[i] = 1;
			}
		}

		//average
		int nrOfExtremesToDrop{ 3 };
		for (int i{ nrOfExtremesToDrop }; i < nrOfSamples - nrOfExtremesToDrop; ++i)
		{
			averageTime += elapsedTimes[i];
		}
		averageTime /= nrOfSamples - (2 * nrOfExtremesToDrop);
		g_Timings.push_back(averageTime);

		//reset
		for (int i{}; i < arraySize; ++i)
		{
			pBigArray[i] = 1;
		}
		averageTime = 0.f;
		elapsedTimes.clear();
	}

	delete[] pBigArray;

	for (auto& timing : g_Timings)
	{
		timing /= 1000;
	}

	g_CacheTrashed = true;
}

void TrashTheCacheObj()
{
	g_TimingsObj.clear();

	const int arraySize{ 10000000 };
	GameObject3D* pBigArray = new GameObject3D[arraySize]{};
	std::vector<long long> elapsedTimes{};
	float averageTime{};
	const int nrOfSamples{ 15 };

	for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
	{
		//Timings
		for (int j{}; j < nrOfSamples; ++j)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			for (int i{}; i < arraySize; i += stepSize)
			{
				pBigArray[i].ID *= 2;
			}

			auto endTime = std::chrono::high_resolution_clock::now();
			long long elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
			elapsedTimes.push_back(elapsedTime);

			//reset
			for (int i{}; i < arraySize; ++i)
			{
				pBigArray[i].ID = 1;
			}
		}

		//average
		int nrOfExtremesToDrop{ 3 };
		for (int i{ nrOfExtremesToDrop }; i < nrOfSamples - nrOfExtremesToDrop; ++i)
		{
			averageTime += elapsedTimes[i];
		}
		averageTime /= nrOfSamples - (2 * nrOfExtremesToDrop);
		g_TimingsObj.push_back(averageTime);

		//reset
		for (int i{}; i < arraySize; ++i)
		{
			pBigArray[i].ID = 1;
		}
		averageTime = 0.f;
		elapsedTimes.clear();
	}

	delete[] pBigArray;

	for (auto& timing : g_TimingsObj)
	{
		timing /= 1000;
	}

	g_CacheTrashedObj = true;
}

void TrashTheCacheObjAlt()
{
	g_TimingsObjAlt.clear();

	const int arraySize{ 10000000 };
	GameObject3DAlt* pBigArray = new GameObject3DAlt[arraySize]{};
	std::vector<long long> elapsedTimes{};
	float averageTime{};
	const int nrOfSamples{ 15 };

	for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
	{
		//Timings
		for (int j{}; j < nrOfSamples; ++j)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			for (int i{}; i < arraySize; i += stepSize)
			{
				pBigArray[i].ID *= 2;
			}

			auto endTime = std::chrono::high_resolution_clock::now();
			long long elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
			elapsedTimes.push_back(elapsedTime);

			//reset
			for (int i{}; i < arraySize; ++i)
			{
				pBigArray[i].ID = 1;
			}
		}

		//average
		int nrOfExtremesToDrop{ 3 };
		for (int i{ nrOfExtremesToDrop }; i < nrOfSamples - nrOfExtremesToDrop; ++i)
		{
			averageTime += elapsedTimes[i];
		}
		averageTime /= nrOfSamples - (2 * nrOfExtremesToDrop);
		g_TimingsObjAlt.push_back(averageTime);

		//reset
		for (int i{}; i < arraySize; ++i)
		{
			pBigArray[i].ID = 1;
		}
		averageTime = 0.f;
		elapsedTimes.clear();
	}

	delete[] pBigArray;

	for (auto& timing : g_TimingsObjAlt)
	{
		timing /= 1000;
	}

	g_CacheTrashedObjAlt = true;
}
