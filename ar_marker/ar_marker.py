import numpy as np
import cv2 as cv


def get_center(corners):
    if corners is None or corners.shape[0] != 4:
        print("Invalid marker corners")
        return None, None

    try:
        # Extract coordinates
        x_coords = [corner[0] for corner in corners]
        y_coords = [corner[1] for corner in corners]

        # Calculate center coordinates
        x_center = sum(x_coords) / len(x_coords)
        y_center = sum(y_coords) / len(y_coords)

        return x_center, y_center
    except Exception as e:
        print(f"Error calculating center: {e}")
        return None, None


def get_area(corner):
    if corner.shape == (4, 2):
        x0, y0 = corner[0]
        x1, y1 = corner[1]
        x2, y2 = corner[2]
        x3, y3 = corner[3]

        # Area calculation using the Shoelace formula
        area = abs((x0 * y1 + x1 * y2 + x2 * y3 + x3 * y0) - (y0 * x1 + y1 * x2 + y2 * x3 + y3 * x0)) / 2
        return area
    else:
        raise ValueError(f"Invalid corner shape: {corner.shape}")


class ArMarkerDetector:
    def __init__(self, aruco_dictionary=cv.aruco.DICT_6X6_250):
        dictionary = cv.aruco.getPredefinedDictionary(aruco_dictionary)
        parameters = cv.aruco.DetectorParameters_create()
        self.detector = cv.aruco.ArucoDetector(dictionary, parameters)

    def get_all_markers(self, image):
        markerCorners, markerIds, rejectedCandidates = self.detector.detectMarkers(image)

        if markerCorners is None:
            markerCorners = []
        if markerIds is None:
            markerIds = []

        return markerCorners, markerIds

    def get_largest_marker(self, image):
        try:
            markerCorners, markerIds = self.get_all_markers(image)

            if not markerCorners or not markerIds:
                raise ValueError("No markers detected or invalid marker data.")

            biggest_index = None
            max_area = 0

            for i, corners in enumerate(markerCorners):
                if corners.shape != (1, 4, 2):
                    print(f"Unexpected corners shape: {corners.shape}")
                    continue

                corner = corners[0]
                try:
                    area = get_area(corner)
                except ValueError as e:
                    print(f"Error calculating area: {e}")
                    continue

                if area > max_area:
                    max_area = area
                    biggest_index = i

            if biggest_index is not None:
                biggest_marker_id = markerIds[biggest_index][0]
                print(f"Largest marker ID: {biggest_marker_id}, Area: {max_area}")
                return markerCorners[biggest_index], markerIds[biggest_index]
            else:
                print("No sufficiently large marker found")
                return None, None

        except Exception as e:
            print(f"Error: {e}")
            return None, None

    def find_marker(self, image, target_id):
        marker_corners, marker_ids = self.get_all_markers(image)

        marker_ids = np.array(marker_ids).flatten() if marker_ids is not None else np.array([])
        marker_corners = np.array(marker_corners) if isinstance(marker_corners, list) else marker_corners

        if len(marker_ids) == 0 or len(marker_corners) == 0:
            print("No markers detected")
            return None, None

        for i, marker_id in enumerate(marker_ids):
            if marker_id == target_id:
                if i < len(marker_corners):
                    print(f"Found marker ID: {marker_id} with corners: {marker_corners[i]}")
                    return marker_corners[i], marker_id

        print(f"Marker ID {target_id} not found")
        return None, None

    def get_marker_center(self, image, target_id):
        marker_corners, marker_id = self.find_marker(image, target_id)
        if marker_corners is None or len(marker_corners) == 0:
            return None, None
        return get_center(marker_corners)

    def get_marker_area(self, image, target_id):
        marker_corners, marker_id = self.find_marker(image, target_id)
        if marker_corners is None or len(marker_corners) == 0:
            return None, None
        return get_area(marker_corners)
