#pragma once 

#include "Solution.h"
#include "GenerationConfig.h"
#include "imgui.h"

#include <GL/gl3w.h>
#include <opencv2/opencv.hpp>

#include <vector>

class SetupView
{
public:
	SetupView(Solution& solution);
    virtual ~SetupView() = default;

    virtual void draw(Solution& solution);

private:
	
	bool showPins_    = true;

    GLuint textureID_;
    cv::Mat image_;


	ImVec2 textureDisplaySize_;


};

