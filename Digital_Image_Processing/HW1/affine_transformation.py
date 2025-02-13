import cv2
import numpy as np

# Scale Matrix
def scale_matrix(scale_x, scale_y):
    return np.array([[scale_x, 0, 0],
                     [0, scale_y, 0],
                     [0, 0, 1]], dtype=np.float32)

# Rotate Matrix
def rotate_matrix(angle_degrees, center_x, center_y):
    angle_radians = np.radians(angle_degrees)
    cos_a = np.cos(angle_radians)
    sin_a = np.sin(angle_radians)

    return np.array([[cos_a, -sin_a, center_x * (1 - cos_a) + center_y * sin_a],
                     [sin_a, cos_a, center_y * (1 - cos_a) - center_x * sin_a],
                     [0, 0, 1]], dtype=np.float32)

# Horizontal Shear Matrix
def horizontal_shear_matrix(shear_factor):
    return np.array([[1, shear_factor, 0],
                     [0, 1, 0],
                     [0, 0, 1]], dtype=np.float32)

# Zoom Matrix
def zoom_matrix(zoom_factor):
    return np.array([[zoom_factor, 0, 0],
                     [0, zoom_factor, 0],
                     [0, 0, 1]], dtype=np.float32)

# Combine Transformations into Affine Transformation Matrix
def affine_transformation_matrix(scale_x, scale_y, angle, shear_factor, zoom_factor, center_x, center_y):
    scale_mat = scale_matrix(scale_x, scale_y)
    rotate_mat = rotate_matrix(angle, center_x, center_y)
    shear_mat = horizontal_shear_matrix(shear_factor)
    zoom_mat = zoom_matrix(zoom_factor)
    
    # Order of matrices: Scale -> Rotate -> Shear -> Zoom
    affine_matrix = shear_mat @ rotate_mat @ scale_mat @ zoom_mat
    return affine_matrix

# Forward Mapping
def forward_mapping(image, transformation_matrix):
    height, width = image.shape[:2]
    transformed_image = np.zeros_like(image)

    for y in range(height):
        for x in range(width):
            original_pos = np.array([x, y, 1], dtype=np.float32)
            new_pos = transformation_matrix @ original_pos
            new_x, new_y = int(new_pos[0]), int(new_pos[1])

            if 0 <= new_x < width and 0 <= new_y < height:
                transformed_image[new_y, new_x] = image[y, x]

    return transformed_image


def backward_mapping(image, transformation_matrix):
    height, width = image.shape[:2]
    
    transformed_image = np.zeros_like(image)

    for y in range(height):
        for x in range(width):
            new_pos = np.array([x, y, 1], dtype=np.float32)
            
            original_pos = np.linalg.inv(transformation_matrix) @ new_pos
            original_x, original_y = int(original_pos[0]), int(original_pos[1])
            
            if 0 <= original_x < width and 0 <= original_y < height:
                transformed_image[y, x] = image[original_y, original_x]

    return transformed_image


def bilinear_interpolation(image, x, y):
    x1 = int(np.floor(x))
    x2 = int(np.ceil(x))
    y1 = int(np.floor(y))
    y2 = int(np.ceil(y))
    
    height, width = image.shape[:2]
    
    if x1 < 0 or x2 < 0 or y1 < 0 or x1 >= width or x2 >= width or y1 >= height or y2 >= height:
        return [0, 0, 0]  
    
    I11 = image[y1, x1]  
    I21 = image[y1, x2]  
    I12 = image[y2, x1]  
    I22 = image[y2, x2]  
    #Interpolation calculations
    f_x1 = (x2 - x) * I11 + (x - x1) * I21
    f_x2 = (x2 - x) * I12 + (x - x1) * I22
    value = (y2 - y) * f_x1 + (y - y1) * f_x2

    return value

def backward_mapping_bilinear(image, transformation_matrix):
    height, width = image.shape[:2]
    
    transformed_image = np.zeros_like(image)

    for y in range(height):
        for x in range(width):
            new_pos = np.array([x, y, 1], dtype=np.float32)
            
            original_pos = np.linalg.inv(transformation_matrix) @ new_pos
            original_x, original_y = original_pos[0], original_pos[1]
            
            transformed_image[y, x] = bilinear_interpolation(image, original_x, original_y)

    return transformed_image



def main():
    
    scale_x, scale_y = 1.0, 1.0
    angle = 60
    shear_factor = 1.4
    zoom_factor = 1.4
    
    
    image = cv2.imread("istanbul.jpg")
    center_x, center_y = image.shape[1] / 2, image.shape[0] / 2

    
    affine_matrix = affine_transformation_matrix(scale_x, scale_y, angle, shear_factor, zoom_factor, center_x, center_y)
    print("Affine Transformation Matrix:\n", affine_matrix)

    
    transformed_image1 = forward_mapping(image, affine_matrix)
    transformed_image2 = backward_mapping(image, affine_matrix)
    transformed_image3 = backward_mapping_bilinear(image, affine_matrix)

    
    cv2.imshow("Original Image", image)
    cv2.imshow("Transformed Image with Forward Mapping", transformed_image1)
    cv2.imshow("Transformed Image with Backward Mapping", transformed_image2)
    cv2.imshow("Transformed Image with Backward Mapping Bilinear Interpolation", transformed_image3)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
