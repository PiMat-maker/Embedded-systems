import numpy as np
import cv2


def gstreamer_pipeline(
        capture_width=1280,
        capture_height=720,
        display_width=1280,
        display_height=720,
        framerate=30,
        flip_method=0,
):
    return (
            "nvarguscamerasrc ! "
            "video/x-raw(memory:NVMM), "
            "width=(int)%d, height=(int)%d, "
            "format=(string)NV12, framerate=(fraction)%d/1 ! "
            "nvvidconv flip-method=%d ! "
            "video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! "
            "videoconvert ! "
            "video/x-raw, format=(string)BGR ! appsink max-buffers=1 drop=true"
            % (
                capture_width,
                capture_height,
                framerate,
                flip_method,
                display_width,
                display_height,
            )
    )


def process(frame, x, y):
    rect_size = 100
    h_sensivity = 20
    s_h = 255
    v_h = 255
    s_l = 100
    v_l = 100
    start_point = (int(y - rect_size / 2), int(x - rect_size / 2))
    end_point = (int(y + rect_size / 2), int(x + rect_size / 2))
    title_color = (255, 255, 0)
    thickness = 2
    rect = cv2.rectangle(frame, start_point, end_point, title_color, thickness)

    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    colors = [[[10, 170], 'red'], [[60], 'green'], [[120], 'blue']]
    result_color = ''
    org = (int(y - rect_size / 2), int(x - rect_size / 2) + 30)
    font = cv2.FONT_HERSHEY_SIMPLEX
    fontScale = 0.7
    for color in colors:
        color_rate = 0
        for degree in color[0]:
            color_upper = np.array([degree + int(h_sensivity / len(color[0])), s_h, v_h])
            color_lower = np.array([degree - int(h_sensivity / len(color[0])), s_l, v_l])
            mask_frame = hsv_frame[start_point[1]:end_point[1] + 1, start_point[0]:end_point[0] + 1]
            mask_green = cv2.inRange(mask_frame, color_lower, color_upper)

            color_rate += np.count_nonzero(mask_green) / (rect_size * rect_size)

        if color_rate > 0.9:
            cv2.putText(rect, color[1], org, font, fontScale, title_color, thickness, cv2.LINE_AA)
            result_color = color[1]

        av_hue = np.average(mask_frame[:, :, 0])
        av_sat = np.average(mask_frame[:, :, 1])
        av_val = np.average(mask_frame[:, :, 2])
        average = [int(av_hue), int(av_sat), int(av_val)]

        #cv2.putText(rect, str(average) + " " + str(color_rate), (10, 220 + int(color[0][0] / 10) ** 2 - 30),
                    #font, fontScale, title_color, thickness, cv2.LINE_AA)

    if result_color == '':
        cv2.putText(rect, 'unknown', org, font, fontScale, title_color, thickness, cv2.LINE_AA)
    return result_color


print('Press 4 to Quit the Application\n')

cap = cv2.VideoCapture(0)

while cap.isOpened():

    ret, frame = cap.read()

    frame = cv2.flip(frame, 180)

    color_1 = process(frame, 120, 90)
    color_2 = process(frame, 120, 240)
    color_3 = process(frame, 120, 390)
    color_4 = process(frame, 120, 540)

    org = (300, 300)
    font = cv2.FONT_HERSHEY_SIMPLEX
    fontScale = 0.7
    if color_1 == 'red' and color_2 == 'green' and color_3 == 'blue' and color_4 == 'green':
        cv2.putText(frame, 'Correct', org, font, fontScale, (0, 255, 0), 2, cv2.LINE_AA)
    else:
        cv2.putText(frame, 'Incorrect', org, font, fontScale, (0, 0, 255), 2, cv2.LINE_AA)

    cv2.imshow('Cam', frame)

    k = cv2.waitKey(1) & 0xFF
    if k == 52:
        print('Good Bye!')
        break

cap.release()
cv2.destroyAllWindows()
