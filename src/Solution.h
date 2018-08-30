#pragma once 

#include "GenerationConfig.h"
#include "imgui.h"
#include <opencv2/opencv.hpp>

#include <vector>


class Solution
{
public:
    Solution(const cv::Mat image);

    virtual void draw();

    bool empty() const {
        return pattern_.empty();
    }

    void clear() {
        pattern_.clear();
    }

    cv::Mat getImage() const {
        return image_;
    }

    int getPasses() const {
        return passes_;
    }

    int getPins() const {
        return pinsOnBoard_.size();
    }

    void push_back(int pin) {
        pattern_.push_back(pin);
    }

    std::vector<int>::iterator begin() {
        return pattern_.begin();
    }

    std::vector<int>::iterator end() {
        return pattern_.end();
    }

    std::vector<int>::const_iterator begin() const {
        return pattern_.cbegin();
    }

    std::vector<int>::const_iterator end() const {
        return pattern_.cend();
    }

    std::vector<int>::const_iterator cbegin() const {
        return pattern_.cbegin();
    }

    std::vector<int>::const_iterator cend() const {
        return pattern_.cend();
    }

	void scaleToDisplay(std::vector<cv::Point2f> const &in, std::vector<ImVec2> &out, float displaySize)
	{
		float const displayRatio = displaySize / boardConfig_.width_;

		int const pinsCount = in.size();
		out.resize(pinsCount);

		float const halfDisplaySize = displaySize / 2.f;

		for (int pin = 0; pin < pinsCount; ++pin)
		{
			ImVec2 &pos = out[pin];
			pos.x = in[pin].x;
			pos.y = in[pin].y;

			pos.x *= displayRatio;
			pos.y *= displayRatio;

			pos.x += halfDisplaySize;
			pos.y += halfDisplaySize;
		}
	}


    void save(const char* file);
    void load(const char* file);

private:
    cv::Mat image_;
    int passes_ = 3000;
	std::vector<int> pattern_;

public:
	std::vector<cv::Point2f> pinsOnBoard_;
	BoardConfig boardConfig_;
};

