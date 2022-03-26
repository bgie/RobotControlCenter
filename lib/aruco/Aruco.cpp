/*  RobotControlCenter
    Copyright (C) 2021-2022 Kuppens Brecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Aruco.h"
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

struct Aruco::Data {
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    cv::Ptr<cv::aruco::DetectorParameters> parameters;
    float markerLengthInMm;
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
};

Aruco::Aruco(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    _d->dictionary = getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    _d->parameters = cv::aruco::DetectorParameters::create();
    _d->parameters->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
    _d->markerLengthInMm = 41.0f;
}

Aruco::~Aruco()
{
}

void Aruco::setCameraMatrix(const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs)
{
    _d->cameraMatrix = cameraMatrix;
    _d->distCoeffs = distCoeffs;
}

Aruco::Markers Aruco::detectMarkers(QImage image) const
{
    Markers result;
    if (!image.size().isEmpty() && !_d->cameraMatrix.empty() && !_d->distCoeffs.empty()) {
        cv::Mat view(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::aruco::detectMarkers(view, _d->dictionary, result.corners, result.ids, _d->parameters, cv::noArray(), _d->cameraMatrix, _d->distCoeffs);
        cv::aruco::estimatePoseSingleMarkers(result.corners, _d->markerLengthInMm, _d->cameraMatrix, _d->distCoeffs, result.rvecs, result.tvecs);
    }
    return result;
}

float Aruco::calc2dAngle(const cv::Vec3d& rvec, const cv::Vec3d& tvec) const
{
    static std::vector<cv::Point3f> axesPoints({ cv::Point3f(0, 0, 0), cv::Point3f(1, 0, 0) });
    std::vector<cv::Point2f> proj;
    projectPoints(axesPoints, rvec, tvec, _d->cameraMatrix, _d->distCoeffs, proj);
    return atan2(proj.at(1).y - proj.at(0).y, proj.at(1).x - proj.at(0).x);
}

void Aruco::drawMarkers(QImage& image, const Aruco::Markers& markers) const
{
    if (!image.size().isEmpty()) {
        cv::Mat view(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
        cv::aruco::drawDetectedMarkers(view, markers.corners, markers.ids);
    }
}

void Aruco::drawPolygon(QImage& image, const QPolygonF& polygon, float z)
{
    static cv::Vec3d nullVector(0.0f, 0.0f, 0.0f);
    static const int width = 3;
    static const cv::Scalar color(255, 255, 0);

    const int count = polygon.size();
    if (count > 1) {
        std::vector<cv::Point3f> points;
        foreach (const QPointF& p, polygon) {
            points.push_back(cv::Point3f(p.x(), p.y(), z));
        }
        std::vector<cv::Point2f> proj;
        projectPoints(points, nullVector, nullVector, _d->cameraMatrix, _d->distCoeffs, proj);
        cv::Mat view(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
        for (int i = 0; i < count; i++) {
            cv::line(view, proj[i], proj[(i + 1) % count], color, width);
        }
    }
}

//const Scalar& color = filtered ? Scalar(0, 255, 255) : Scalar(0, 0, 255);
//const int width = filtered ? 2 : 5;
//std::vector<Point3f> axesPoints;
//axesPoints.push_back(Point3f(0, 0, 0));
//axesPoints.push_back(Point3f(ANNOTATE_AXIS_LENGTH, 0, 0));
////if (!filtered) {
//axesPoints.push_back(Point3f(ANNOTATE_CENTER_POINT_SIZE, 0, 0));
////}
//std::vector<Point2f> imagePoints;
//projectPoints(axesPoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

//cv::line(img, imagePoints[0], imagePoints[1], color, width);

////if (!filtered) {
//Point2f centerSize = (imagePoints[2] - imagePoints[0]);
//const int FIXEDPOINT_BITS = 8;
//const int FIXEDPOINT_MULTIPLIER = (1 << FIXEDPOINT_BITS);
//int radius = static_cast<int>(round(FIXEDPOINT_MULTIPLIER * sqrt(centerSize.x * centerSize.x + centerSize.y * centerSize.y)));
//Point2i center(round(imagePoints[0].x * FIXEDPOINT_MULTIPLIER), round(imagePoints[0].y * FIXEDPOINT_MULTIPLIER));
//cv::circle(img, center, radius, color, cv::FILLED, cv::FILLED, FIXEDPOINT_BITS);
////}

void Aruco::generateMarkerImageFiles(QString path) const
{
    // TODO

    //    cv::Mat markerImage;
    //    for (int i = 0; i < 50; ++i) {
    //        drawMarker(_d->dictionary, i, 200, markerImage, 1);
    //        imwrite(QString("marker%1.png").arg(i).toLatin1().constData(), markerImage);
    //    }
}
