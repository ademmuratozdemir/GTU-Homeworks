import cv2
import numpy as np
def find_neighbors(x, y, image):
#         """Find connected neighbors using 8-connectivity."""
    height, width = image.shape
    neighbors = []
    
    # Check all 8 neighboring pixels
    for dx in [-1, 0, 1]:
        for dy in [-1, 0, 1]:
            if dx == 0 and dy == 0:
                continue
            
            new_x, new_y = x + dx, y + dy
            
            # Check boundaries
            if 0 <= new_x < width and 0 <= new_y < height:
                if image[new_y, new_x] == 0:  # If pixel is black
                    neighbors.append((new_x, new_y))
    
    return neighbors
def connected_components(image, min_radius=2, max_radius=10):
    """
    Find connected components in binary image with size constraints.
    Returns labels and centroids.
    
    Parameters:
    - image: binary image
    - min_radius: minimum radius of dots to detect
    - max_radius: maximum radius of dots to detect
    """
    height, width = image.shape
    labels = np.zeros((height, width), dtype=int)
    current_label = 1
    centroids = []
    
    # First pass: label all pixels
    for y in range(height):
        for x in range(width):
            if image[y, x] == 0 and labels[y, x] == 0:  # Unlabeled black pixel
                # Start a new component
                pixels_to_check = [(x, y)]
                pixel_coords = []
                
                # Label all connected pixels
                while pixels_to_check:
                    curr_x, curr_y = pixels_to_check.pop()
                    
                    if labels[curr_y, curr_x] == 0:
                        labels[curr_y, curr_x] = current_label
                        pixel_coords.append((curr_x, curr_y))
                        
                        # Add neighbors to check
                        neighbors = find_neighbors(curr_x, curr_y, image)
                        pixels_to_check.extend(neighbors)
                
                # Calculate component size and centroid
                if pixel_coords:
                    x_coords, y_coords = zip(*pixel_coords)
                    
                    # Calculate approximate radius from area
                    area = len(pixel_coords)
                    approx_radius = np.sqrt(area / np.pi)
                    
                    # Only keep components within radius range
                    if min_radius <= approx_radius <= max_radius:
                        centroid_x = int(np.mean(x_coords))
                        centroid_y = int(np.mean(y_coords))
                        centroids.append((centroid_x, centroid_y))
                    else:
                        # Reset labels for components outside size range
                        for px, py in pixel_coords:
                            labels[py, px] = 0
                
                current_label += 1
    
    return labels, centroids

def find_circle_centers(binary_image, min_radius=2, max_radius=10):
    """
    Find centers of circles in binary image.
    
    Parameters:
    - binary_image: input binary image
    - min_radius: minimum radius of dots to detect
    - max_radius: maximum radius of dots to detect
    
    Returns:
    - labels and centroids of detected dots
    """
    image = binary_image.copy()
    
    # Find components and their centers with size constraints
    labels, centroids = connected_components(image, min_radius, max_radius)
    
    return labels, centroids

def visualize_labels_and_centers(original_image, labels, centroids):
    """
    Visualize both labels and centroids on the image.
    Different colors for different labels and marks centroids.
    """
    # Create RGB image for visualization
    height, width = original_image.shape[:2]
    result = np.zeros((height, width, 3), dtype=np.uint8)
    
    # Create random colors for each label
    n_labels = len(centroids) + 1  # +1 for background
    colors = np.random.randint(0, 255, size=(n_labels, 3), dtype=np.uint8)
    colors[0] = [255, 255, 255]  # background color = white
    
    # Color each component with a different color
    for y in range(height):
        for x in range(width):
            result[y, x] = colors[labels[y, x]]
    
    # Draw centroids as red dots
    for (x, y) in centroids:
        x, y = int(x), int(y)
        cv2.circle(result, (x, y), 3, (0, 0, 255), -1)  # red dot
        
    return result
