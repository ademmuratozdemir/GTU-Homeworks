import numpy as np
import cv2
from tqdm import tqdm

def adaptive_threshold(image, max_value, adaptive_method, threshold_type, block_size, C):
    """
    Custom implementation of adaptive thresholding with progress bar.

    Args:
        image (np.ndarray): Input grayscale image.
        max_value (int): Maximum value to use for thresholding.
        adaptive_method (str): Adaptive method to calculate threshold ("mean" or "gaussian").
        threshold_type (str): Thresholding type ("binary" or "binary_inv").
        block_size (int): Size of the pixel neighborhood (must be odd and >= 3).
        C (int): Constant subtracted from the mean or weighted mean.

    Returns:
        np.ndarray: Thresholded binary image.
    """
    if block_size % 2 == 0 or block_size < 3:
        raise ValueError("block_size must be an odd number and >= 3.")

    # Pad the image to handle edges
    pad_size = block_size // 2
    padded_image = np.pad(image, pad_size, mode='reflect')

    # Output image
    output = np.zeros_like(image, dtype=np.uint8)

    # Pre-compute Gaussian kernel if needed
    if adaptive_method == "gaussian":
        kernel = np.outer(
            np.exp(-0.5 * (np.arange(block_size) - pad_size)**2 / (block_size / 6)**2),
            np.exp(-0.5 * (np.arange(block_size) - pad_size)**2 / (block_size / 6)**2)
        )
        kernel /= kernel.sum()

    # Total number of pixels to process
    total_pixels = image.shape[0] * image.shape[1]
    
    # Initialize progress bar
    with tqdm(total=total_pixels, desc="Processing", unit="pixels") as pbar:
        # Compute the threshold for each pixel
        for y in range(image.shape[0]):
            for x in range(image.shape[1]):
                # Extract the local block
                block = padded_image[y:y + block_size, x:x + block_size]

                # Compute the threshold value based on the adaptive method
                if adaptive_method == "mean":
                    threshold_value = block.mean()
                elif adaptive_method == "gaussian":
                    threshold_value = (block * kernel).sum()
                else:
                    raise ValueError("Invalid adaptive method. Use 'mean' or 'gaussian'.")

                # Apply thresholding
                if threshold_type == "binary":
                    output[y, x] = max_value if image[y, x] > (threshold_value - C) else 0
                elif threshold_type == "binary_inv":
                    output[y, x] = 0 if image[y, x] > (threshold_value - C) else max_value
                else:
                    raise ValueError("Invalid threshold type. Use 'binary' or 'binary_inv'.")
                
                # Update progress bar
                pbar.update(1)

    return output

# Example usage
if __name__ == "__main__":
    # Load an example image
    img = cv2.imread('example.jpg', cv2.IMREAD_GRAYSCALE)
    
    # Apply adaptive thresholding
    result = adaptive_threshold(
        image=img,
        max_value=255,
        adaptive_method="gaussian",
        threshold_type="binary",
        block_size=11,
        C=2
    )
    
    # Display results
    cv2.imshow('Original', img)
    cv2.imshow('Adaptive Threshold', result)
    cv2.waitKey(0)
    cv2.destroyAllWindows()