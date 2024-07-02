

from PIL import Image
import numpy as np

def image_to_rgba2222_bitmap(image_path):
    # Open the image file
    with Image.open(image_path) as img:
        # Convert the image to RGBA format
        img = img.convert('RGBA')
        # Create a numpy array of the image
        bitmap = np.array(img)
        
        # Convert the bitmap to RGBA2222 format
        rgba2222_bitmap = []
        for row in bitmap:
            rgba2222_row = []
            for pixel in row:
                # Scale down the RGBA values from 8-bit to 2-bit
                r = (pixel[0] >> 6) & 0b11  # Keep only the 2 most significant bits
                g = (pixel[1] >> 6) & 0b11  # Keep only the 2 most significant bits
                b = (pixel[2] >> 6) & 0b11  # Keep only the 2 most significant bits
                a = (pixel[3] >> 6) & 0b11  # Keep only the 2 most significant bits
                # Combine the 2-bit values into one 8-bit number
                rgba2222_pixel = (a << 6) | (b << 4) | (g << 2) | r
                rgba2222_row.append(rgba2222_pixel)
            rgba2222_bitmap.append(rgba2222_row)
        
    return rgba2222_bitmap

def save_rgba2222_bitmap_to_file(rgba2222_bitmap, output_file):
    # Write the RGBA2222 bitmap array to a file
    with open(output_file, 'w') as f:
        for row in rgba2222_bitmap:
            row_str = ','.join(f'0x{pixel:02x}' for pixel in row)
            f.write(row_str + ',\n')

if __name__ == "__main__":
    # Path to the image file
    image_path = 'red_light_60x52.jpg'
    # Path to the output file
    output_file = 'red_light_60x52.txt'
    
    # Convert image to RGBA2222 bitmap
    rgba2222_bitmap = image_to_rgba2222_bitmap(image_path)
    
    # Save the RGBA2222 bitmap array to a file
    save_rgba2222_bitmap_to_file(rgba2222_bitmap, output_file)

