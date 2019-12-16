max_speed = int(input())
light_count = int(input())
distance = []
duration = []

for i in range(light_count):
    d, t = input().split()
    distance.append(int(d))
    duration.append(int(t))

s = max_speed

while s > 1:
    pass_all = True

    for i in range(light_count):
        if (int)(3600 * distance[i] / (s * duration[i] * 1000)) % 2 == 1:
            pass_all = False
            break

    if pass_all:
        break
    else:
        s -= 1

print(s)

#cherche s <= max_speed_m_s : (( distance[i] / s ) / duraton[i]) % 2 == 0 
#0 1 . . 4 5 . .
