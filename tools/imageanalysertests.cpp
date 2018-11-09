#include "imageanalysertests.h"

ImageAnalyserTests::ImageAnalyserTests() :
    images(std::vector<cv::Mat>())
{

}

ImageAnalyserTests::~ImageAnalyserTests()
{

}

int ImageAnalyserTests::startTests()
{
    std::cout<<"Start tests.\n";

    int ret =0;
    std::cout<<"test_showMatrice.\n";
    ret = test_showMatrice();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_showMatrice failed.\n";
    }

    std::cout<<"test_applyGreyScaleCondition.\n";
    ret = test_applyGreyScaleCondition();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_applyGreyScaleCondition failed.\n";
    }

    std::cout<<"test_toQImage.\n";
    ret = test_toQImage();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_toQImage failed.\n";
    }

    std::cout<<"test_toMatCV.\n";
    ret = test_toMatCV();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_toMatCV failed.\n";
    }

    std::cout<<"test_separateImage.\n";
    ret = test_separateImage();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_separateImage failed.\n";
    }

    std::cout<<"test_computeGreyScale.\n";
    ret = test_computeGreyScale();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeGreyScale failed.\n";
    }

    std::cout<<"test_computeGaussianBlur.\n";
    ret = test_computeGaussianBlur();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeGaussianBlur failed.\n";
    }

    std::cout<<"test_computeSobel.\n";
    ret = test_computeSobel();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeSobel failed.\n";
    }

    std::cout<<"test_computeLaplacian.\n";
    ret = test_computeLaplacian();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeLaplacian failed.\n";
    }

    std::cout<<"test_computeBMDisparity.\n";
    ret = test_computeBMDisparity();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeBMDisparity failed.\n";
    }

    std::cout<<"test_computeSGBMDisparity.\n";
    ret = test_computeSGBMDisparity();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeSGBMDisparity failed.\n";
    }

    std::cout<<"test_computeEfficiency.\n";
    ret = test_computeEfficiency();
    if (ret !=0)
    {
        std::cout<<"\tERROR: test_computeEfficiency failed.\n";
    }

    std::cout<<"Tests finished.\n";
    return 0;
}

int ImageAnalyserTests::test_showMatrice()
{
    cv::Mat image = cv::imread(prefix_images+"leafs_of_autumn_by_stereojiyone-d32jkqv.jpg", CV_LOAD_IMAGE_COLOR);

    if(! image.data )
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    ImageAnalyser::showMatrice("TestShowMatrice",image);
    char c;
    std::cin >> c;
    cv::destroyWindow("TestShowMatrice");

    return 0;
}

int ImageAnalyserTests::test_applyGreyScaleCondition()
{
    return 0;
}

int ImageAnalyserTests::test_toQImage()
{
    return 0;
}

int ImageAnalyserTests::test_toMatCV()
{
    return 0;
}

int ImageAnalyserTests::test_separateImage()
{
    return 0;
}

int ImageAnalyserTests::test_computeGreyScale()
{
    return 0;
}

int ImageAnalyserTests::test_computeGaussianBlur()
{
    return 0;
}

int ImageAnalyserTests::test_computeSobel()
{
    return 0;
}

int ImageAnalyserTests::test_computeLaplacian()
{
    return 0;
}

int ImageAnalyserTests::test_computeBMDisparity()
{
    std::string test;
    std::cin >> test;
    std::cout << test << std::endl;
    return 0;
}

int ImageAnalyserTests::test_computeSGBMDisparity()
{
    std::string test;
    std::cin >> test;
    std::cout << test << std::endl;
    return 0;
}

int ImageAnalyserTests::test_computeEfficiency()
{
    return 0;
}
