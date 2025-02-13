import numpy as np
import cv2
from cluster import * 
def create_gaussian_kernel(kernel_size=5, sigma=1.0):
    """
    Create a 2D Gaussian kernel for blurring.
    """
    # Create coordinate matrices
    ax = np.linspace(-(kernel_size - 1)/2., (kernel_size - 1)/2., kernel_size)
    xx, yy = np.meshgrid(ax, ax)
    
    # Calculate Gaussian values
    kernel = np.exp(-0.5 * (np.square(xx) + np.square(yy)) / np.square(sigma))
    
    # Normalize the kernel
    return kernel / np.sum(kernel)
def medianBlur(image, ksize):
    # Ensure ksize is odd
    if ksize % 2 == 0:
        raise ValueError("Kernel size must be odd")
    
    # Get image dimensions
    if len(image.shape) == 3:
        height, width, channels = image.shape
    else:
        height, width = image.shape
        channels = 1
        image = image.reshape(height, width, 1)
    
    # Calculate padding size
    pad = ksize // 2
    
    # Create output image
    output = np.zeros_like(image)
    
    # Process each pixel
    for y in range(height):
        for x in range(width):
            for c in range(channels):
                # Get neighborhood
                neighborhood = []
                for i in range(-pad, pad + 1):
                    for j in range(-pad, pad + 1):
                        # Calculate neighbor coordinates
                        ny, nx = y + i, x + j
                        
                        # Handle border by mirroring
                        if ny < 0:
                            ny = abs(ny)
                        elif ny >= height:
                            ny = 2 * height - ny - 2
                            
                        if nx < 0:
                            nx = abs(nx)
                        elif nx >= width:
                            nx = 2 * width - nx - 2
                        
                        neighborhood.append(image[ny, nx, c])
                
                # Calculate median
                output[y, x, c] = np.median(neighborhood)
    
    # Return same shape as input
    if channels == 1:
        output = output.reshape(height, width)
        
    return output.astype(np.uint8)
def gaussian_blur(image, kernel_size=5, sigma=1.0):
    """
    Apply Gaussian blur using convolution.
    """
    # Get kernel
    kernel = create_gaussian_kernel(kernel_size, sigma)
    
    # Pad the image
    pad_size = kernel_size // 2
    if len(image.shape) == 3:
        padded = np.pad(image, ((pad_size, pad_size), (pad_size, pad_size), (0, 0)), mode='reflect')
    else:
        padded = np.pad(image, ((pad_size, pad_size), (pad_size, pad_size)), mode='reflect')
    
    # Initialize output array
    output = np.zeros_like(image, dtype=np.float32)
    
    # Apply convolution
    if len(image.shape) == 3:
        for i in range(3):  # For each color channel
            for y in range(image.shape[0]):
                for x in range(image.shape[1]):
                    output[y, x, i] = np.sum(
                        padded[y:y+kernel_size, x:x+kernel_size, i] * kernel
                    )
    else:
        for y in range(image.shape[0]):
            for x in range(image.shape[1]):
                output[y, x] = np.sum(
                    padded[y:y+kernel_size, x:x+kernel_size] * kernel
                )
    
    return np.clip(output, 0, 255).astype(np.uint8)

def bgr_to_gray(image):
    """
    Convert BGR image to grayscale using weighted sum method.
    """
    # Standard weights for BGR to grayscale conversion
    weights = np.array([0.114, 0.587, 0.299])
    
    # Calculate weighted sum
    grayscale = np.dot(image[..., :3], weights)
    
    return grayscale.astype(np.uint8)

def gray_to_bgr(image):
    """
    Convert grayscale image to BGR by replicating the channel.
    """
    if len(image.shape) == 3 and image.shape[2] == 1:
        image = image[:, :, 0]
    
    # Convert to uint8 if not already
    if image.dtype != np.uint8:
        image = (image * 255).astype(np.uint8)
    
    # Create 3-channel BGR image by repeating the grayscale channel
    bgr_image = np.dstack((image, image, image))
    
    return bgr_image

def bounding_rect(contour):
    """
    Custom implementation of cv2.boundingRect.
    
    Args:
        contour (np.ndarray): Contour points.

    Returns:
        tuple: (x, y, w, h) - top-left corner (x, y), width (w), and height (h).
    """
    x_coords = [point[0] for point in contour]
    y_coords = [point[1] for point in contour]

    x_min, x_max = min(x_coords), max(x_coords)
    y_min, y_max = min(y_coords), max(y_coords)

    width = x_max - x_min
    height = y_max - y_min

    return x_min, y_min, width, height

def arc_length(contour, closed):
    """
    Custom implementation of cv2.arcLength.
    
    Args:
        contour (np.ndarray): Contour points.
        closed (bool): Whether the contour is closed.

    Returns:
        float: Perimeter of the contour.
    """
    length = 0.0
    for i in range(len(contour)):
        if i == len(contour) - 1 and not closed:
            break

        pt1 = np.array(contour[i])
        pt2 = np.array(contour[(i + 1) % len(contour)])
        length += np.linalg.norm(pt2 - pt1)

    return length
def normalize_contour(contour):
    """
    Normalizes contour format to (N, 2) array of float32.
    
    Args:
        contour: Input contour in any format (N, 1, 2) or (N, 2)
        
    Returns:
        np.ndarray: Normalized contour points as (N, 2) array
    """
    # Convert to numpy array if not already
    points = np.asarray(contour, dtype=np.float32)
    
    # Handle different contour formats
    if points.shape[-1] == 2:  # Check if last dimension is 2 (x,y coordinates)
        if len(points.shape) == 3:  # Format: (N, 1, 2)
            points = points.reshape(-1, 2)
        elif len(points.shape) == 2:  # Format: (N, 2)
            pass
        else:
            raise ValueError(f"Unexpected contour shape: {points.shape}")
    else:
        raise ValueError(f"Contour must have 2 coordinates per point, got shape: {points.shape}")
    
    return points
def approx_poly_dp(contour, epsilon, closed):
    """
    Custom implementation of cv2.approxPolyDP.
    
    Args:
        contour (np.ndarray): Contour points.
        epsilon (float): Approximation accuracy.
        closed (bool): Whether the contour is closed.

    Returns:
        list: Approximated contour points.
    """
    def rdp(points, epsilon):
        """Ramer-Douglas-Peucker algorithm."""
        if len(points) < 3:
            return points

        start, end = np.array(points[0]), np.array(points[-1])
        max_dist = -1
        index = -1

        for i in range(1, len(points) - 1):
            pt = np.array(points[i])
            dist = np.abs(np.cross(end - start, pt - start) / np.linalg.norm(end - start))
            if dist > max_dist:
                max_dist = dist
                index = i

        if max_dist > epsilon:
            left = rdp(points[:index + 1], epsilon)
            right = rdp(points[index:], epsilon)
            return left[:-1] + right

        return [start.tolist(), end.tolist()]

def find_rectangle_groups(cnts):
    """
    Find groups of rectangles with similar x coordinates and check for count of 57
    """
    # First filter rectangles meeting basic criteria (x < 40 and w > h)
    rectangles_all=[]
    for cnt in cnts:
        rectangles_all.append(bounding_rect(cnt))
    filtered_rects = [rect for rect in rectangles_all if rect[0] < 40 and rect[2] > rect[3]]
    
    # Group rectangles by similar x coordinates
    x_groups = {}
    
    for rect in filtered_rects:
        x, y, w, h = rect
        # Check each x-1, x, and x+1
        found_group = False
        for base_x in range(x-1, x+2):
            if base_x in x_groups:
                x_groups[base_x].append(rect)
                found_group = True
                break
        
        if not found_group:
            x_groups[x] = [rect]
    
    # Print and check each group's count
    found_target = False
    print("\nChecking groups for count of 57:")
    for base_x, rects in sorted(x_groups.items()):
        count = len(rects)
        print(f"Group x={base_x}: {count} rectangles")
        
        if count == 57:
            print(f"Found group with exactly 57 rectangles at x={base_x}!")
            found_target = True
            # You might want to store or process this specific group
            target_group = rects
            return rects
    
    if not found_target:
        print("\nNo group with exactly 57 rectangles found.")
        # Print closest groups
        closest = min(x_groups.items(), key=lambda x: abs(len(x[1]) - 57))
        print(f"Closest group has {len(closest[1])} rectangles at x={closest[0]}")

    return x_groups

def find_centers_clustering(binary_image,rectangle,min_radius,max_radius):
    cropped_questions=crop_image(binary_image,rectangle[0],rectangle[1],rectangle[2],rectangle[3])
    
    cv2.imshow("crop ques",cropped_questions)
    cv2.waitKey(0)
    cropped_questions_not = cv2.bitwise_not(cropped_questions)

    cv2.imshow("crop ques not",cropped_questions_not)
    cv2.waitKey(0)
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(5,5))
    cropped_questions_not=cv2.morphologyEx(cropped_questions_not,cv2.MORPH_OPEN,kernel)
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(8,8))
    cropped_questions_not=cv2.morphologyEx(cropped_questions_not,cv2.MORPH_OPEN,kernel)

    cv2.imshow("crop questions not procceseed", cropped_questions_not)
    cv2.waitKey(0)

    cropped_questions_processed=cv2.bitwise_not(cropped_questions_not)

    cv2.imshow("crop questions clustering input", cropped_questions_processed)
    cv2.waitKey(0)
    
    labels,centers=find_circle_centers(cropped_questions_processed,2,12)
    cluster_visulaize=visualize_labels_and_centers(cropped_questions_processed,labels,centers)
    cv2.imshow(" clustering out", cluster_visulaize)
    cv2.waitKey(0)
    print(centers)
    print(len(centers))
    return centers


def map_answers_to_new_rectangle(original_corners, new_corners, answers):
    """
    Maps answers' pixel positions from the original rectangle to a new rectangle.

    Parameters:
        original_corners (list): List of 4 (x, y) tuples representing the corners of the original rectangle.
        new_corners (list): List of 4 (x, y) tuples representing the corners of the new rectangle.
        answers (list): List of (x, y) tuples representing the answers' pixel positions in the original rectangle.

    Returns:
        list: List of (x, y) tuples representing the answers' pixel positions in the new rectangle.
    """
    # Ensure inputs are numpy arrays
    original_corners = np.array(original_corners, dtype=np.float32)
    new_corners = np.array(new_corners, dtype=np.float32)

    # Compute the perspective transformation matrix
    transformation_matrix = cv2.getPerspectiveTransform(original_corners, new_corners)

    # Convert answers to homogeneous coordinates for transformation
    answers_array = np.array(answers, dtype=np.float32).reshape(-1, 1, 2)

    # Apply the perspective transformation
    transformed_answers_pixels = cv2.perspectiveTransform(answers_array, transformation_matrix)

    # Reshape the result back to a list of tuples
    return [tuple(point[0]) for point in transformed_answers_pixels]



def calculate_average_intensity(binary_image, gray_image,rectangles, mark_index,center,questions, radius=2):
    """
    Calculate the average intensity in a circular region around a center.

    Parameters:
        binary_image (numpy.ndarray): The binary image.
        center (tuple): The (x, y) center of the circular region.
        radius (int): The radius of the circular region.

    Returns:
        float: The average intensity within the circular region.
    """

    if questions:
        if mark_index%25==0:
            control_rectangle_index=0
        else:
            control_rectangle_index=25-((mark_index)%25)
        control_rectangle_x,control_rectangle_y,control_rectangle_w,control_rectangle_h=rectangles[control_rectangle_index]
        new_y=int(control_rectangle_y+control_rectangle_h/2)
        mask = np.zeros_like(binary_image, dtype=np.uint8)
        # gray_image=cv2.circle(gray_image,(int(center[0]), int(new_y)),1,(255,255,0),1)
        # cv2.circle(mask, (int(center[0]), int(new_y)), radius, 255, -1)
        values = binary_image[mask == 255]
        # return float(np.mean(values)) if len(values) > 0 else 0
        x,y,w,h=int(center[0])-4,control_rectangle_y,8,control_rectangle_h
        roi = binary_image[y:y+h, x:x+w]
        gray_image[y:y+h, x:x+w]=[255,255,0]

    # Calculate mean value
        mean_value = np.mean(roi)
        return mean_value
       
    else:
        control_rectangle_x,control_rectangle_y,control_rectangle_w,control_rectangle_h=rectangles[mark_index]
        new_y=int(control_rectangle_y+control_rectangle_h/2)
        mask = np.zeros_like(binary_image, dtype=np.uint8)
        gray_image=cv2.circle(gray_image,(int(center[0]), int(new_y)),1,(255,255,0),1)

        # cv2.circle(mask, (int(center[0]), int(new_y)), radius, 255, -1)
        values = binary_image[mask == 255]
        # return float(np.mean(values)) if len(values) > 0 else 0
        x,y,w,h=int(center[0])-4,control_rectangle_y,8,control_rectangle_h
        roi = binary_image[y:y+h, x:x+w]
        gray_image[y:y+h, x:x+w]=[255,255,0]
    # Calculate mean value
        mean_value = np.mean(roi)
        return mean_value
    
def crop_image(img, x, y, width, height):
    # Read the image
    
    # Ensure coordinates are within image boundaries
    height_img, width_img = img.shape[:2]
    x = max(0, min(x, width_img))
    y = max(0, min(y, height_img))
    width = max(0, min(width, width_img - x))
    height = max(0, min(height, height_img - y))
    
    # Crop the image
    cropped = img[y:y+height, x:x+width]
    
    return cropped