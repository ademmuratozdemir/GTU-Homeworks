import numpy as np
import cv2 
import pandas as pd
from contour import *
from canny import *
from cluster import *
from helpers import *
from adaptive_threshold import *


import os
script_dir = os.path.dirname(os.path.abspath(__file__))

read_answers_csv = os.path.join(script_dir, 'true_answers.csv')
csv_path = os.path.join(script_dir, 'answers.pixels.csv')
uploaded_csv = os.path.join(script_dir, 'adjusted_circle_grid.csv')
read_answers_df = pd.read_csv(read_answers_csv)
uploaded_df = pd.read_csv(uploaded_csv, sep=';', index_col=0, encoding='latin1')

read_answers_dict = dict(zip(read_answers_df['Answer Number'], read_answers_df['Answer']))

original_image = cv2.imread(os.path.join(script_dir, 'example.jpg'))

 

gtu_name_rectangle_ratio=((544-288)/(409-50))
gtu_question_rectangle_ratio=(543-253)/(726-423)
name_rectangle_index=0
question_rectangle_index=1
resized_image=None


blurred_image=None


resized_image=cv2.resize(original_image,(600,800), interpolation=cv2.INTER_AREA)


final_output=resized_image.copy()
gray_image=bgr_to_gray(resized_image)
gray_image_intensity_process=gray_image.copy()
gray_image_intensity_process[15:22, 12:17] = 255

for_output=resized_image.copy()



adaptive=adaptive_threshold(gray_image,255,adaptive_method="mean", threshold_type="binary",
        block_size=3, C=1)

adaptive2=adaptive_threshold(gray_image,255,adaptive_method="mean", threshold_type="binary",
        block_size=11, C=6)


adaptive_output_for_control=adaptive2.copy()


adaptive=medianBlur(adaptive2,1)

cv2.imshow("gray", gray_image)
cv2.waitKey(0)

cv2.imshow("adaptive", adaptive)
cv2.waitKey(0)

edges_reference_rectangles=apply_canny_edge_detection(False,adaptive_output_for_control,adaptive_output_for_control, 
                             kernal_size=5, 
                             low_threshold_ratio=0.05, 
                             high_threshold_ratio=0.20, 
                             gradient_estimation_filter_type="sobel")


edges=apply_canny_edge_detection(False,adaptive,adaptive, 
                             kernal_size=5, 
                             low_threshold_ratio=0.05, 
                             high_threshold_ratio=0.20, 
                             gradient_estimation_filter_type="sobel")


if edges.dtype == np.float16 or edges.dtype == np.int32:
    edges = cv2.normalize(edges, None, 0, 255, cv2.NORM_MINMAX)
    edges = np.uint8(edges)
cv2.imshow("edges_from canny",edges)
cv2.waitKey(0)

contours, _ = findContours(edges)
contours = [np.array(contour, dtype=np.int32) for contour in contours]
con=gray_image.copy()
cv2.drawContours(con, contours, -1, (0, 255, 0), 1)  # Yeşil renkte (BGR formatında) çizin
cv2.imshow("contours ",con)
cv2.waitKey(0)

contours_reference_rectangles, _ = findContours(edges_reference_rectangles)
contours_reference_rectangles = [np.array(contour, dtype=np.int32) for contour in contours_reference_rectangles]




rectangles_reference=[]
gray_image=cv2.cvtColor(gray_image, cv2.COLOR_GRAY2BGR)

rect_reference=gray_image.copy()

rect_reference2=gray_image.copy()
groups = find_rectangle_groups(contours_reference_rectangles)

if isinstance(groups,list) and len(groups)==57:
    rectangles_reference=groups
elif isinstance(groups,dict):
    rectangles_reference = min(groups.items(), key=lambda x: abs(len(x[1]) - 57))

rectangles_reference.sort(key=lambda rect: rect[1],reverse=True)

for rctn in rectangles_reference:
        x, y, w, h = rctn
    
        cv2.rectangle(gray_image, (x, y), (x + w, y + h), (255, 255, 0), 1)
        gray_image=cv2.line(gray_image,(0,y),(gray_image.shape[1],y),(255,0,0),1)
        gray_image=cv2.line(gray_image,(0,y+h),(gray_image.shape[1],y+h),(255,0,0),1)
rectangles_all=[]
min_area =  500 # Define the minimum area threshold
largest_area=0
question_rectangle=None

for contour in contours:
    # Get the bounding rectangle for the contour
    x, y, w, h = bounding_rect(contour)
    area = w * h

    # Draw the rectangle if the area is larger than the threshold
    if area > min_area:
        epsilon = 0.02 * arc_length(contour, True)
        approx = cv2.approxPolyDP(contour, epsilon, True)

    # Check if it has 4 vertices and is convex
        if len(approx) == 4 :
            cv2.rectangle(gray_image, (x, y), (x + w, y + h), (0, 255, 0), 1)  # Green rectangle
            print(x,y,w,h)
            rectangles_all.append((x,y,w,h))

cv2.imshow("gray_rectangeles2", gray_image)
cv2.waitKey(0)

max_sum = -float("inf")

max_sum_name = -float("inf")

question_rectangle=None
name_rectangle=None

rectangles_all = sorted(rectangles_all, key=lambda rect: rect[2] * rect[3],reverse=True)
name_rectangle=rectangles_all[name_rectangle_index]
question_rectangle=rectangles_all[question_rectangle_index]
if gtu_name_rectangle_ratio*0.98<(name_rectangle[2]/name_rectangle[3])<gtu_name_rectangle_ratio*1.02:
    if not abs(name_rectangle[2] - question_rectangle[2])<=3:
        # question_rectangle[2]=name_rectangle[2]+name_rectangle[0]-question_rectangle[0]
        question_rectangle=(question_rectangle[0],question_rectangle[1],name_rectangle[2]+name_rectangle[0]-question_rectangle[0],question_rectangle[3])
elif gtu_question_rectangle_ratio*0.98<(question_rectangle[2]/question_rectangle[3])<gtu_question_rectangle_ratio*1.02:
        name_rectangle=(name_rectangle[0],name_rectangle[1],question_rectangle[2]+question_rectangle[0]-name_rectangle[0],name_rectangle[3])




answers_df = pd.read_csv(csv_path)
answer_sheet_pixels = list(zip(answers_df['X'], answers_df['Y']))

questions = answers_df['Question'].tolist()
options = answers_df['Option'].tolist()

question_rectangle_original_corners = [(1, 0), (730, 1), (732, 822), (2, 822)]

 # Define the new rectangle corners (top-left, top-right, bottom-right, bottom-left)
x,y,w,h=question_rectangle
question_rectangle_new_corners = [(x,y), (x+w, y), (x+w, y+h),(x, y+h)]
#print(new_corners)



rectangles_reference.sort(key=lambda rect: rect[1],reverse=True)

question_contol_rectangles=rectangles_reference[:25]
centers=find_centers_clustering(adaptive2,question_rectangle,2,12)

centers_global=[]
for center in centers:
    centers_global.append((center[0]+question_rectangle[0],center[1]+question_rectangle[1]))

result = {}
    
    # Initialize result dict with rectangle y values
for rect in question_contol_rectangles:
    y = rect[1]  # y coordinate of rectangle
    result[y] = []
    
# For each y, find closest 4 centers
for y in result.keys():
    # Calculate distances from this y to all centers
    distances = [(center, abs(center[1] - y)) for center in centers_global if abs(center[1] - y) < 5]
    # Sort by distance and take closest 4
    closest = sorted(distances, key=lambda x: x[1])[:4]
    result[y] = [center for center, dist in closest]
i=0
for rectangle in rectangles_reference:
    x,y,w,h=rectangle
    rect_reference2=cv2.line(rect_reference2,(0,y),(rect_reference2.shape[1],y),(255,0,0),1)
    rect_reference2=cv2.line(rect_reference2,(0,y+h),(rect_reference2.shape[1],y+h),(255,0,0),1)

cv2.imshow("before rect process",rect_reference2)
cv2.waitKey(0)
for y in result.keys():
    original_rect=rectangles_reference[i]
    centers=result[rectangles_reference[i][1]]
    first_elements = [t[1] for t in centers]
    new_y=sum(first_elements) / len(first_elements)
    rectangles_reference[i]=(original_rect[0],int(new_y-(original_rect[3]/2)),original_rect[2],original_rect[3])
    i+=1
for rectangle in rectangles_reference:
    x,y,w,h=rectangle
    rect_reference=cv2.line(rect_reference,(0,y),(rect_reference.shape[1],y),(255,0,0),1)
    rect_reference=cv2.line(rect_reference,(0,y+h),(rect_reference.shape[1],y+h),(255,0,0),1)

cv2.imshow("after rect process",rect_reference)
cv2.waitKey(0)

transformed_answers_pixels = map_answers_to_new_rectangle(question_rectangle_original_corners, question_rectangle_new_corners, answer_sheet_pixels)
# print(transformed_answers_pixels)
# for point in transformed_answers_pixels:
#     gray_image=cv2.circle(gray_image,(int(point[0]),int(point[1])),1,(255,255,0),1)
    # Combine data into a dictionary structure
question_dict = {}
correct_answer=0
for i in range(len(answer_sheet_pixels)):
    question_num = questions[i]
    option = options[i]
    # print(question_num,i)
    if question_num not in question_dict:
        question_dict[question_num] = {}
    question_dict[question_num][option] = {
        "Original": answer_sheet_pixels[i],
        "Transformed": transformed_answers_pixels[i],
       "Intensity": calculate_average_intensity(adaptive2,final_output,rectangles_reference,question_num,transformed_answers_pixels[i],True)

    }
    if option=='E':
        min_intensity=min(question_dict[question_num].items(), key=lambda item: item[1]["Intensity"])
        #print(min_intensity)
        question_dict[question_num]["Marked"]=min_intensity[0]
        true_option = read_answers_dict.get(question_num-1)
        is_correct = question_dict[question_num]['Marked'] == true_option
        question_dict[question_num]["is_correct"]=is_correct
        
        if is_correct:
            correct_answer+=1
        
        #print(is_correct,question_num,question_dict[question_num]["Marked"],true_option)
#print(correct_answer)
# print(question_dict)
for question_num in sorted(question_dict.keys()):
    marked = question_dict[question_num]["Marked"]
    is_correct = question_dict[question_num]["is_correct"]
    
    print(f"{question_num:<10} {marked:<10} {'✓' if is_correct else '✗'}")
print(f"True Marks: {correct_answer} False Marks: {100-correct_answer}")
for question_num in sorted(question_dict.keys()):
    marked = question_dict[question_num]
    # print(f"{question_num} {marked}")
# for entry in transformed_answers_pixels:
#         x, y = int(entry[0]), int(entry[1])
#         cv2.circle(resized_image, (x, y), 3, (255, 255, 0), -1)

def transform_point_to_new_rectangle(original_corners, new_corners, point):
    """
    Maps a single point's pixel position from the original rectangle to a new rectangle.

    """
    import numpy as np
    import cv2

    # Ensure inputs are numpy arrays
    original_corners = np.array(original_corners, dtype=np.float32)
    new_corners = np.array(new_corners, dtype=np.float32)
    point_array = np.array([[point]], dtype=np.float32)

    # Compute the transformation matrix
    transformation_matrix = cv2.getPerspectiveTransform(original_corners, new_corners)

    # Apply the perspective transformation
    transformed_point = cv2.perspectiveTransform(point_array, transformation_matrix)

    # Extract and return the transformed point as a tuple
    return tuple(transformed_point[0][0])
original_name_corners=[(26,33),(896,40),(900,1308),(30,1305)]




x_n,y_n,w_n,h_n=name_rectangle
name_rect_image=[(x_n,y_n),(x_n+w_n,y_n),(x_n+w_n,y_n+h_n),(x_n,y_n+h_n)]
cv2.rectangle(gray_image,(x_n,y_n),(x_n+w_n,y_n+h_n),(255,255,0),1)
# Initialize the dictionary to store the data
column_dictionary = {}
turkish_alphabet = ['A', 'B', 'C', 'Ç', 'D', 'E', 'F', 'G', 'Ğ', 'H', 'I', 'İ', 'J', 'K', 'L', 'M', 'N', 'O', 'Ö', 'P', 'R', 'S', 'Ş', 'T', 'U', 'Ü', 'V', 'Y', 'Z']

# Process each column and row
# aa=cv2.imread("images/ex_13.png")
# cv2.imshow("aaaa",aa)
# cv2.waitKey(0)
chars=[]
columns={}
a=0
rectangles_reference.sort(key=lambda rect: rect[1],reverse=False)
for column in uploaded_df.columns:
    column_data = {}  # To store real and transformed pixel values for each entry in the column
    alphabet_index = 0  # Reset alphabet index for each column
    
    for row in uploaded_df.index:
        cell = uploaded_df.at[row, column]
        if pd.notna(cell):  # If the cell is not empty
            # Parse the coordinates from the string
            coords = cell.strip("()").split(", ")
            x, y = int(coords[0]), int(coords[1])
            
            chars.append((x,y))
           # print(x,y)
            # print(alphabet_index,a)
            (x1,y1)=transform_point_to_new_rectangle(original_name_corners,name_rect_image,(x,y))
            column_data[str(turkish_alphabet[alphabet_index% len(turkish_alphabet)])]={}
            column_data[str(turkish_alphabet[alphabet_index% len(turkish_alphabet)])]={
                
                "real_pixel": (x, y),
                "transformed_pixel": (x1,y1),
                "Intensity" : calculate_average_intensity(adaptive2,final_output,rectangles_reference,alphabet_index,(x1,y1),False,2)
            }
            # cv2.circle(gray_image, (int(x1), int(y1)), 3, 255, -1)
            
            #print(column_data[str(turkish_alphabet[alphabet_index% len(turkish_alphabet)])])
            alphabet_index+=1
           # print(len(column_data   ))
    
    columns[a]=column_data
    
    a+=1

# print(columns)

name=[]
for key in columns.keys():
    min_intensity=min(columns[key].items(), key=lambda item: item[1]["Intensity"])
    # print(min_intensity)
    data=min_intensity[1]
    if min_intensity[1]['Intensity']<75:
        columns[key]["Min Intensity"]=min_intensity[0]
        name.append(min_intensity[0])
    else:
        columns[key]["Min Intensity"]=" "
        name.append(" ")
    # column_dictionary[column] = column_data
    # print(column_data)


cv2.imshow("final_output", final_output)
# for question_num in sorted(question_dict.keys()):
#     marked = question_dict[question_num]["Marked"]
#     is_correct = question_dict[question_num]["is_correct"]
    
#     print(f"{question_num:<10} {marked:<10} {'✓' if is_correct else '✗'}")


cv2.waitKey(0)
print(name)
rectangles_reference.sort(key=lambda rect: rect[1])

