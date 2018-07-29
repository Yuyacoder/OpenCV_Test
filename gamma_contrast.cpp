#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
int

main(int argc, char *argv[])
{
	Mat src_img = cv::imread("lena.jpg", 1);
	if (!src_img.data) return -1;

	// チャンネルを分離する
	vector<Mat> planes;
	split(src_img, planes);

	// 入力パラメータ
	// コントラスト調整用（各チャンネルでのシグモイド関数のゲイン）
	float as[] = { -0.5f, 2.0f, 6.0f };
	// ガンマ補正用（各チャンネルでのシグマ値）
	float gammas[] = { 0.5f, 0.8f, 0.3f };

	uchar lut[256];
	for (int ch = 0; ch < 3; ch++) {
		float a = as[ch];
		float gamma = gammas[ch];
		Mat original = planes[ch];

		// 出力画像
		Mat result;

		// コントラスト調整
		// ルックアップテーブル作成
		for (int i = 0; i < 256; i++)
			lut[i] = 255.0 / (1 + exp(-a*(i - 128) / 255));
		LUT(original, Mat(Size(256, 1), CV_8U, lut), result);


		// ガンマ補正
		if (gamma != 1) {
			double gm = 1.0 / gamma;
			// ルックアップテーブル作成
			for (int i = 0; i < 256; i++)
				lut[i] = pow(1.0*i / 255, gm) * 255;

			LUT(result, Mat(Size(256, 1), CV_8U, lut), result);
		}

		planes[ch] = result;
	}
	// チャンネルを合成する
	Mat resultColor;
	merge(planes, resultColor);

	// 元画像の表示
	imshow("original", src_img);
	//imwrite("images\\res.jpg", resultColor);
	// 結果画像の表示
	imshow("result", resultColor);
	waitKey(0);
}