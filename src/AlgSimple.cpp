#include "imgui.h"

#include "AlgSimple.h"
#include "Solution.h"

#include <opencv2/opencv.hpp>

#include <map>

AlgSimple::AlgSimple() :
    calculating_{false},
    currentPass_{0}
{
}

void AlgSimple::draw()
{
    ImGui::Checkbox("More than one pass per pin pair", &morePassesPerPinPair_);
    ImGui::SliderInt("Penalty", &penalty_, 0, 255);
    ImGui::SliderInt("Min Space", &minSpace_, 0, 50);
}

void AlgSimple::calculate(Solution& solution)
{
    calculating_ = true;
    solution.clear();

    int currentPin= 0;
    solution.push_back(currentPin);

    cv::Mat image;
    cv::cvtColor(solution.getImage(), image, cv::COLOR_BGR2GRAY);
    int pins = solution.getPins();

    std::map<std::pair<int, int>, bool> strings;

	std::vector<ImVec2> pinsOnDisplay;

	solution.scaleToDisplay(solution.pinsOnBoard_, pinsOnDisplay, image.cols);


    for(currentPass_ = 1; currentPass_ <= solution.getPasses(); currentPass_++)
    {
        float maxScore = 0;
        int nextPin = 0;
        int prevPin = 0;


        cv::Point2d start{ pinsOnDisplay[currentPin].x, pinsOnDisplay[currentPin].y };

        for (int pin = ((currentPin + 1 + minSpace_) % pins); ((pin + minSpace_) % pins) != currentPin; pin = (pin + 1) % pins)
        {
            if (!morePassesPerPinPair_ ||
                (strings.find(std::make_pair(prevPin, pin)) == strings.end() &&
                 strings.find(std::make_pair(pin, prevPin)) == strings.end()))
            {

				cv::Point2d end{ pinsOnDisplay[pin].x, pinsOnDisplay[pin].y };

                cv::LineIterator line(image, start, end, 8);
                float sum = 0;
                for (int index = 0; index < line.count; ++index, line++)
                {
                    sum += 255 - (uchar)**line;
                }

                float score = sum / line.count;
                if (score > maxScore)
                {
                    maxScore = score;
                    nextPin = pin;
                }
            }
        }

		cv::Point2d end{ pinsOnDisplay[nextPin].x, pinsOnDisplay[nextPin].y };


        cv::LineIterator line(image, start, end, 8);
        for (int index = 0; index < line.count; ++index, line++)
        {
            image.at<uchar>(line.pos()) = image.at<uchar>(line.pos()) + penalty_ > 255
                ? 255 : image.at<uchar>(line.pos()) + penalty_;
        }

        prevPin = currentPin;
        currentPin = nextPin;
        solution.push_back(currentPin);
        strings.emplace(std::make_pair(std::make_pair(prevPin, currentPin), true));
    }

    calculating_ = false;
}
