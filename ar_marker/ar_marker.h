#ifndef INCLUDED_AR_MARKER_H
#define INCLUDED_AR_MARKER_H

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <tuple>
#include <vector>

class ArMarkerDetector {
public:
    explicit ArMarkerDetector(int dictionaryId = cv::aruco::DICT_6X6_250) {
        cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(dictionaryId);
        cv::aruco::DetectorParameters parameters;
        detector = cv::aruco::ArucoDetector(dictionary, parameters);
    }

    std::tuple<std::vector<std::vector<cv::Point2f>>, std::vector<int>> get_all_markers(const cv::Mat &image) {
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        std::vector<int> markerIds;
        detector.detectMarkers(image, markerCorners, markerIds, rejectedCandidates);
        return {markerCorners, markerIds};
    }

    std::tuple<std::vector<cv::Point2f>, int> getLargestMarker(const cv::Mat &image) {
        std::vector<std::vector<cv::Point2f>> markerCorners;
        std::vector<int> markerIds;
        std::tie(markerCorners, markerIds) = get_all_markers(image);

        if (markerCorners.empty() || markerIds.empty()) {
            throw std::runtime_error("Cannot find largest marker since no Ar Marker was detected");
        }

        int biggestIndex = 0;
        double maxArea = 0.0;

        for (int i = 0; i < markerCorners.size(); ++i) {
            double area = cv::contourArea(markerCorners[i]);
            if (area > maxArea) {
                maxArea = area;
                biggestIndex = i;
            }
        }

        return {markerCorners[biggestIndex], markerIds[biggestIndex]};
    }

    std::tuple<std::vector<cv::Point2f>, int> findMarker(const cv::Mat &image, const int targetId) {
        std::vector<std::vector<cv::Point2f>> markerCorners;
        std::vector<int> markerIds;
        std::tie(markerCorners, markerIds) = get_all_markers(image);

        if (markerCorners.empty() || markerIds.empty()) {
            throw std::runtime_error("Cannot find marker since no Ar Marker was detected");
        }

        for (size_t i = 0; i < markerIds.size(); ++i) {
            if (markerIds[i] == targetId) {
                return {markerCorners[i], markerIds[i]};
            }
        }

        throw std::runtime_error("Cannot find marker with the specified ID");
    }

    double getMarkerArea(cv::Mat &image, int target_id) {
        try {
            std::vector<cv::Point2f> corners;
            int markerId;
            std::tie(corners, markerId) = findMarker(image, target_id);

            if (corners.size() < 4) { // At least 4 points are needed to form a contour
                throw std::runtime_error("Insufficient number of points to calculate area");
            }

            return cv::contourArea(corners);
        } catch (const std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw;
        }
    }

    std::tuple<double, double> getMarkerCenter(cv::Mat &image, int target_id) {
        try {
            std::vector<cv::Point2f> corners;
            int markerId;
            std::tie(corners, markerId) = findMarker(image, target_id);

            if (corners.empty()) {
                throw std::runtime_error("No corners found for the specified marker");
            }

            double x_sum = 0.0;
            double y_sum = 0.0;

            for (const auto& corner : corners) {
                x_sum += corner.x;
                y_sum += corner.y;
            }

            return {x_sum / static_cast<double>(corners.size()), y_sum / static_cast<double>(corners.size())};
        } catch (const std::runtime_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw;
        }
    }

private:
    cv::aruco::ArucoDetector detector;
};

#endif
