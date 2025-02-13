import cv2 as cv
import numpy as np
import copy
from tqdm import tqdm
import time

def load_image(image_name):
    image = cv.imread(cv.samples.findFile(image_name))
    if image is None:
        sys.exit("Could not read the image.")
    return image

def convert_to_gray_scale(rgb_image):
    r, g, b = rgb_image[:,:,0], rgb_image[:,:,1], rgb_image[:,:,2]
    gray_image = 0.2989 * r + 0.5870 * g + 0.1140 * b
    return gray_image

def apply_filtering(input_image, kernal):
    output_image = []
    kernal_size = len(kernal)
    kernal_half = kernal_size // 2
    rows_count = len(input_image)
    columns_count = len(input_image[0])

    # Show progress for padding operation
    print("Padding image...")
    image_copy = copy.deepcopy(input_image)

    for i in tqdm(range(rows_count), desc="Padding rows"):
        for j in range(kernal_half):
            image_copy[i].insert(0, input_image[i][-1-j])
            image_copy[i].append(input_image[i][j])
    
    for i in range(kernal_half):
        image_copy.append(image_copy[2*i])
        image_copy.insert(0, image_copy[-2-2*i].copy())

    new_rows_count = len(image_copy)
    new_columns_count = len(image_copy[0])

    print("Applying convolution...")
    # Show progress for convolution operation
    for i in tqdm(range(kernal_half, new_rows_count - kernal_half), desc="Processing rows"):
        output_row = []
        for j in range(kernal_half, new_columns_count - kernal_half):
            sum = 0
            for x in range(len(kernal)):
                for y in range(len(kernal)):
                    x1 = i + x - kernal_half
                    y1 = j + y - kernal_half
                    sum += image_copy[x1][y1] * kernal[x][y]
            output_row.append(sum)
        output_image.append(output_row)

    return output_image

def get_gaussian_kernel(kernal_size, sigma=1):
    print("Generating Gaussian kernel...")
    gaussian_kernal = np.zeros((kernal_size, kernal_size), np.float32)
    size = kernal_size//2

    for x in tqdm(range(-size, size+1), desc="Computing kernel"):
        for y in range(-size, size+1):
            a = 1/(2*np.pi*(sigma**2))
            b = np.exp(-(x**2 + y**2)/(2* sigma**2))
            gaussian_kernal[x+size, y+size] = a*b
    return gaussian_kernal/gaussian_kernal.sum()

def gradient_estimate(image, gradient_estimation_filter_type):
    print("Estimating gradients...")
    if (gradient_estimation_filter_type=="sobel"):
        Mx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], np.float32)
        My = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]], np.float32)
    elif (gradient_estimation_filter_type=="prewitt"):
        Mx = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]], np.float32)
        My = np.array([[1, 1, 1], [0, 0, 0], [-1, -1, -1]], np.float32)
    else:
        Mx = np.array([[0, 1], [-1, 0]], np.float32)
        My = np.array([[1, 0], [0, -1]], np.float32)

    print("Computing X gradient...")
    X = apply_filtering(image, Mx)
    print("Computing Y gradient...")
    Y = apply_filtering(image, My)

    G = np.hypot(X, Y)
    G = G / G.max() * 255
    theta = np.arctan2(Y, X)
    
    return (G, theta)

def non_maxima_suppression(image, gradient_direction):
    print("Applying non-maximum suppression...")
    rows_count = len(image)
    columns_count = len(image[0])

    output_image = np.zeros((rows_count, columns_count), dtype=np.int32)
    theta = gradient_direction * 180. / np.pi
    theta[theta < 0] += 180
 
    for i in tqdm(range(1, rows_count-1), desc="Processing pixels"):
        for j in range(1, columns_count-1):
            next = 255
            previous = 255
            if (0 <= theta[i,j] < 22.5) or (157.5 <= theta[i,j] <= 180):
                next = image[i, j+1]
                previous = image[i, j-1]
            elif (22.5 <= theta[i,j] < 67.5):
                next = image[i+1, j-1]
                previous = image[i-1, j+1]
            elif (67.5 <= theta[i,j] < 112.5):
                next = image[i+1, j]
                previous = image[i-1, j]
            elif (112.5 <= theta[i,j] < 157.5):
                next = image[i-1, j-1]
                previous = image[i+1, j+1]

            if (image[i,j] >= next) and (image[i,j] >= previous):
                output_image[i,j] = image[i,j]
            else:
                output_image[i,j] = 0
    
    return output_image

def double_threshold(image, low_threshold_ratio, high_threshold_ratio):
    print("Applying double threshold...")
    high_threshold = image.max() * high_threshold_ratio
    low_threshold = high_threshold * low_threshold_ratio
    
    rows_count = len(image)
    columns_count = len(image[0])
    output_image = np.zeros((rows_count, columns_count), dtype=np.int32)
    
    weak = np.int32(25)
    strong = np.int32(255)

    strong_i = []
    strong_j = []
    weak_i = [] 
    weak_j = []
    
    for i in tqdm(range(len(image)), desc="Thresholding pixels"):
        for j in range(len(image[0])):
            if (image[i,j]>=high_threshold):
                strong_i.append(i)
                strong_j.append(j)
            if ((image[i,j] <= high_threshold) & (image[i,j] >= low_threshold)):
                weak_i.append(i)
                weak_j.append(j)
                
    strong_i = np.array(strong_i)
    strong_j = np.array(strong_j)
    weak_i = np.array(weak_i)
    weak_j = np.array(weak_j)
    
    output_image[strong_i, strong_j] = strong
    output_image[weak_i, weak_j] = weak
    
    return output_image

def hysteresis_edge_track(image):
    print("Performing edge tracking...")
    weak = np.int32(25)
    strong = np.int32(255)

    rows_count = len(image)
    columns_count = len(image[0])
    
    for i in tqdm(range(1, rows_count-1), desc="Tracking edges"):
        for j in range(1, columns_count-1):
            if (image[i,j] == weak):
                if ((image[i+1, j-1] == strong) or (image[i+1, j] == strong) or (image[i+1, j+1] == strong)
                    or (image[i, j-1] == strong) or (image[i, j+1] == strong)
                    or (image[i-1, j-1] == strong) or (image[i-1, j] == strong) or (image[i-1, j+1] == strong)):
                    image[i, j] = strong
                else:
                    image[i, j] = 0
    return image

def apply_canny_edge_detection(file_path,image1,image_name, kernal_size=3, 
                             low_threshold_ratio=0.05, 
                             high_threshold_ratio=0.09, 
                             gradient_estimation_filter_type="sobel"):
    
    start_time = time.time()
    print("Starting Canny edge detection...")

    print("\nStep 1/6: Loading and converting image to grayscale...")
    image=None
    if file_path:
        image = load_image(image_name)
    else:
        image=image1
    # gray_scaled_image = convert_to_gray_scale(image)
    gray_scaled_image=image
    print("\nStep 2/6: Applying Gaussian filter...")
    kernal = get_gaussian_kernel(kernal_size)
    image_without_noise = apply_filtering(gray_scaled_image.tolist(), kernal)

    print("\nStep 3/6: Estimating gradients...")
    assert (gradient_estimation_filter_type in ["sobel", "prewitt", "robert"]), \
           "gradient estimation filter type should be [\"prewitt\", \"sobel\", \"robert\"]"
    G, theta = gradient_estimate(image_without_noise, gradient_estimation_filter_type)

    print("\nStep 4/6: Applying non-maximum suppression...")
    image_with_thin_edges = non_maxima_suppression(G, theta)
    cv.imwrite("ImageAfterNon-MaximaSuppression1.jpg", np.array(image_with_thin_edges))

    print("\nStep 5/6: Applying double threshold...")
    final_image = double_threshold(image_with_thin_edges, low_threshold_ratio, high_threshold_ratio)
    cv.imwrite("ImageAfterApplyDoubleThreshold1.jpg", np.array(final_image))

    print("\nStep 6/6: Applying hysteresis edge tracking...")
    img = hysteresis_edge_track(final_image)
    cv.imwrite("FinalImage1.jpg", np.array(img))

    print(f"\nCanny edge detection completed in {time.time() - start_time:.2f} seconds")
    return final_image
# Example usage
if __name__ == "__main__":
    apply_canny_edge_detection("/home/ibu/image_processing/images/ex_14.jpeg", 
                             kernal_size=5, 
                             low_threshold_ratio=0.05, 
                             high_threshold_ratio=0.20, 
                             gradient_estimation_filter_type="sobel")