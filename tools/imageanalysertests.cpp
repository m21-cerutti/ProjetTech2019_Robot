#include "imageanalysertests.h"

ImageAnalyserTests::ImageAnalyserTests() :
    images(std::vector())
{

}

ImageAnalyserTests::~ImageAnalyserTests()
{

}

int ImageAnalyserTests::startTests()
{
    qDebug()<<"Start tests.";

    int ret =0;
    qDebug()<<"test_showMatrice.";
    ret = test_showMatrice();
    if (ret !=0)
    {
        qDebug()<<"test_showMatrice failed.";
    }

    qDebug()<<"test_applyGreyScaleCondition.";
   ret = test_applyGreyScaleCondition();
   if (ret !=0)
   {
       qDebug()<<"test_applyGreyScaleCondition failed.";
   }

    qDebug()<<"test_toQImage.";
   ret = test_toQImage();
   if (ret !=0)
   {
       qDebug()<<"test_toQImage failed.";
   }

    qDebug()<<"test_toMatCV.";
   ret = test_toMatCV();
   if (ret !=0)
   {
       qDebug()<<"test_toMatCV failed.";
   }

    qDebug()<<"test_separateImage.";
   ret = test_separateImage();
   if (ret !=0)
   {
       qDebug()<<"test_separateImage failed.";
   }

    qDebug()<<"test_computeGreyScale.";
   ret = test_computeGreyScale();
   if (ret !=0)
   {
       qDebug()<<"test_computeGreyScale failed.";
   }

    qDebug()<<"test_computeGaussianBlur.";
   ret = test_computeGaussianBlur();
   if (ret !=0)
   {
       qDebug()<<"test_computeGaussianBlur failed.";
   }

    qDebug()<<"test_computeSobel.";
   ret = test_computeSobel();
   if (ret !=0)
   {
       qDebug()<<"test_computeSobel failed.";
   }

    qDebug()<<"test_computeLaplacian.";
   ret = test_computeLaplacian();
   if (ret !=0)
   {
       qDebug()<<"test_computeLaplacian failed.";
   }

    qDebug()<<"test_computeBMDisparity.";
   ret = test_computeBMDisparity();
   if (ret !=0)
   {
       qDebug()<<"test_computeBMDisparity failed.";
   }

    qDebug()<<"test_computeSGBMDisparity.";
   ret = test_computeSGBMDisparity();
   if (ret !=0)
   {
       qDebug()<<"test_computeSGBMDisparity failed.";
   }

    qDebug()<<"test_computeEfficiency.";
   ret = test_computeEfficiency();
   if (ret !=0)
   {
       qDebug()<<"test_computeEfficiency failed.";
   }

    qDebug()<<"Tests finished.";
    return 0;
}

int ImageAnalyserTests::test_showMatrice()
{

}

int ImageAnalyserTests::test_applyGreyScaleCondition()
{

}

int ImageAnalyserTests::test_toQImage()
{

}

int ImageAnalyserTests::test_toMatCV()
{

}

int ImageAnalyserTests::test_separateImage()
{

}

int ImageAnalyserTests::test_computeGreyScale()
{

}

int ImageAnalyserTests::test_computeGaussianBlur()
{

}

int ImageAnalyserTests::test_computeSobel()
{

}

int ImageAnalyserTests::test_computeLaplacian()
{

}

int ImageAnalyserTests::test_computeBMDisparity()
{

}

int ImageAnalyserTests::test_computeSGBMDisparity()
{

}

int ImageAnalyserTests::test_computeEfficiency()
{

}
