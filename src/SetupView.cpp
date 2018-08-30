#include "SetupView.h"

#include <sstream>

#define M_PIF float(M_PI)

template<typename T> static T Clamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }

class GeneratePinsPosition
{
protected:
	 
	BoardConfig const &boardConfig_;

public:

	GeneratePinsPosition(BoardConfig const &boardConfig) :
		boardConfig_(boardConfig)
	{}

	virtual std::string getName() const = 0;
	virtual bool draw(void) = 0; // return true if needed to be generated
	virtual void generate(std::vector<cv::Point2f> &) const = 0;
};

class GeneratePinsCircle : public GeneratePinsPosition
{
	int pinsCount_ = 200;

public:

	GeneratePinsCircle(BoardConfig const &boardConfig) :
		GeneratePinsPosition(boardConfig)
	{

	}


	bool draw(void) override
	{
		bool ret = false;

		ret |= ImGui::InputInt("Pins Count", &pinsCount_);
		pinsCount_ = Clamp(pinsCount_, 10, 1000);

		return ret;
	}

	std::string getName() const override
	{
		return "Simple Circle";
	}

	void generate(std::vector<cv::Point2f> &pinsOnBoard) const override
	{
		pinsOnBoard.clear();
		pinsOnBoard.resize(pinsCount_);

		float const pinsCountf = float(pinsCount_);

		float const ray = boardConfig_.width_ / 2.f - boardConfig_.border_;

		for (int pin = 0; pin < pinsCount_; ++pin)
		{
			float const pinf = float(pin);

			float rad = 2.f * M_PIF / pinsCountf * pinf;

			pinsOnBoard[pin].x = cosf(rad) * ray;
			pinsOnBoard[pin].y = sinf(rad) * ray;
		}
	}
};

GeneratePinsPosition *gs_ptobedeleted_GeneratePins = nullptr;


SetupView::SetupView(Solution& solution)
{
    cv::Mat image = solution.getImage();
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image.cols,
                 image.rows,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 image.ptr());

	textureDisplaySize_.x = 500; // image.cols;
	textureDisplaySize_.y = 500; // image.rows;

	if (nullptr == gs_ptobedeleted_GeneratePins)
	{
		gs_ptobedeleted_GeneratePins = new GeneratePinsCircle(solution.boardConfig_);
	}

	gs_ptobedeleted_GeneratePins->generate(solution.pinsOnBoard_);
}

void SetupView::draw(Solution& solution)
{
	if (ImGui::Begin("Setup View", nullptr, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_AlwaysAutoResize)) 
	{
		bool generatePinsPos = false;

		ImGui::Checkbox("Show Pins", &showPins_);
		generatePinsPos |= ImGui::InputFloat("Board size", &solution.boardConfig_.width_, 1, 10, "%0.0f mm");
		generatePinsPos |= ImGui::InputFloat("Border", &solution.boardConfig_.border_, 1, 10, "%0.0f mm");

		solution.boardConfig_.width_ = Clamp(solution.boardConfig_.width_, 100.f, 500.f);
		solution.boardConfig_.border_ = Clamp(solution.boardConfig_.border_, 0.f, (solution.boardConfig_.width_ / 2.f) * .9f);

		ImGui::Separator();

		generatePinsPos |= gs_ptobedeleted_GeneratePins->draw();

		if (generatePinsPos)
		{
			gs_ptobedeleted_GeneratePins->generate(solution.pinsOnBoard_);

		}

		auto drawList = ImGui::GetWindowDrawList();
		auto start = ImGui::GetCursorScreenPos();

		ImGui::Image(reinterpret_cast<ImTextureID>(textureID_), textureDisplaySize_);

		if (showPins_) {

			std::vector<ImVec2> pinsOnDisplay;

			solution.scaleToDisplay(solution.pinsOnBoard_, pinsOnDisplay, textureDisplaySize_.x);

			for (auto pin : pinsOnDisplay)
			{
				pin.x += start.x;
				pin.y += start.y;

				drawList->AddCircleFilled(pin, solution.boardConfig_.pinThickness_, ImColor(255, 100, 0), 8);
			}
		}
	}

	ImGui::End();
}
