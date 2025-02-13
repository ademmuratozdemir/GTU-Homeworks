import numpy as np
import cv2



def findContours(binary_image):
    """
    Implements a simple version of Suzuki and Abe's algorithm for finding contours in binary images.
    
    Args:
        binary_image (np.ndarray): Binary image where contours are to be detected.

    Returns:
        contours (list of list of tuple): List of contours, where each contour is a list of (x, y) coordinates.
        hierarchy (np.ndarray): Information about image topology; (next, previous, first_child, parent).
    """
    # Ensure the image is binary
    binary_image = (binary_image > 0).astype(np.uint8)
    
    height, width = binary_image.shape
    contours = []
    hierarchy = []

    visited = np.zeros_like(binary_image, dtype=bool)

    def follow_border(start_x, start_y, label):
        """Follow the border of a binary component."""
        contour = []
        directions = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]  # 8-neighborhood

        x, y = start_x, start_y
        first_direction = 0

        while True:
            contour.append((x, y))
            visited[y, x] = True

            found_next = False
            for i in range(len(directions)):
                dir_idx = (first_direction + i) % len(directions)
                dx, dy = directions[dir_idx]
                nx, ny = x + dx, y + dy

                if 0 <= nx < width and 0 <= ny < height and binary_image[ny, nx] == 1:
                    if not visited[ny, nx]:
                        x, y = nx, ny
                        first_direction = (dir_idx + 5) % len(directions)  # Reverse the direction
                        found_next = True
                        break

            if not found_next:
                break

            if (x, y) == (start_x, start_y):
                break

        return contour

    for y in range(height):
        for x in range(width):
            if binary_image[y, x] == 1 and not visited[y, x]:
                contour = follow_border(x, y, len(contours) + 1)
                if contour:
                    contours.append(contour)
                    # Append dummy hierarchy data for now
                    hierarchy.append([-1, -1, -1, -1])

    hierarchy = np.array(hierarchy, dtype=np.int32)
    return contours, hierarchy

# Example usage
# if __name__ == "__main__":
#     # binary_image = np.array([
#     #     [0, 0, 0, 0, 0],
#     #     [0, 1, 1, 1, 0],
#     #     [0, 1, 0, 1, 0],
#     #     [0, 1, 1, 1, 0],
#     #     [0, 0, 0, 0, 0]
#     # ], dtype=np.uint8)

#     image_path = "foto1.png"
# image = cv2.imread(image_path, cv2.IMREAD_COLOR)  # Orijinal renkli görüntüyü yükle
# binary_image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)  # Grayscale görüntüyü yükle
# cv2.imshow("binary_image",binary_image)
# cv2.waitKey(0)

# # Görüntü başarıyla yüklendi mi kontrol et
# if image is None or binary_image is None:
#     print("Hata: Görüntü yüklenemedi. Dosya yolunu veya dosyanın bütünlüğünü kontrol edin.")
#     exit()

# # İkili görüntüye dönüştürme
# _, binary_image = cv2.threshold(binary_image, 127, 255, cv2.THRESH_BINARY)

# # Sınırları tespit et
# contours, hierarchy = findContours(binary_image)
# contours = [np.array(contour, dtype=np.int32) for contour in contours]
# # Sınırları çizmek için orijinal görüntüyü kopyalayın
# contoured_image = image.copy()

# # Sınırları çiz
# cv2.drawContours(contoured_image, contours, -1, (0, 255, 0), 1)  # Yeşil renkte (BGR formatında) çizin

# # Görüntüyü göster
# cv2.imshow("Contours", contoured_image)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

# # Görüntüyü kaydetmek isterseniz
# cv2.imwrite("/home/ademmurat/Downloads/contoured_image.jpg", contoured_image)
