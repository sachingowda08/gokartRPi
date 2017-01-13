from pubnub import Pubnub
import math
import sys, signal
import os
import pygame
import time
import random
pubnub = Pubnub(
    publish_key = "pub-c-981f15af-a53d-4141-adb5-2d1eed8e4c64",
    subscribe_key = "sub-c-ffc34cc4-d2ca-11e6-979a-02ee2ddab7fe")

os.environ["SDL_FBDEV"] = "/dev/fb1"
driver = 'fbcon'
os.putenv('SDL_VIDEODRIVER',driver)
pygame.display.init()
size = (pygame.display.Info().current_w/2, pygame.display.Info().current_h/2)

print "Framebuffer size: %d x %d" % (size[0], size[1])
screen = pygame.display.set_mode(size, pygame.FULLSCREEN)
screen.fill((0, 0, 0))
pygame.font.init()
pygame.display.update()
yellow = (255,255, 0)
screen.fill(yellow)
pygame.display.update()

#time.sleep(2)
speed=0
rpm=0
temp=0
lat=0
lng=0;
fuel=0
size2=(size[0]/2,size[1]/2)
background=pygame.Surface(screen.get_size())
background.fill((200,200,200))
font=pygame.font.SysFont("comicsansms",50)
text=font.render(" ",1,(255,255,255))
screen.blit(text,(320 - text.get_width() // 2,0))
def signal_handler(signal, frame):
    print("\nprogram exiting gracefully")
    sys.exit(0)
print "render kar diya bhayaa"
while 1:
    

    values=str.split(",")
    i=0
    v=[]
    def is_number(s):
    	try:
        	float(s)
        	return True
    	except ValueError:
    		return False
    for value in values:
	a=value.split("=")[1]
	if is_number(a) and a != "NaN":
		v.append(a)
	else:
		v.append('0')
    temp=float(v[0])
    lat=float(v[1])
    lng=float(v[2])
    speed=float(v[3])
    rpm=float(v[4])
    
    font=pygame.font.Font(None, 36)
    text=font.render(" ",1,(255,255,255))
    screen.blit(text, size2)
    
    rp="RPM  "+str(rpm)
    rp=font.render(rp,2,(255,255,255))
    screen.blit(rp, (0,0))

    tmp="TEMP  "+str(temp)
    tmp=font.render(tmp,2,(255,255,255))
    screen.blit(tmp, (0,50))

    fl="FUEL  "+str(fuel)
    fl=font.render(fl,2,(255,255,255))
    screen.blit(fl, (0,100))

    spd="SPEED  "+str(speed)
    spd=font.render(spd,2,(255,255,255))	
    screen.blit(spd, (0,150))

    eonRPM={'eon':{
        'RPM': rpm
        }
        }
    eonTEMP={'eon':{
        'TEMPERATURE': temp
        }
        }
    
    eonSPEED={'eon':{
        'SPEED': speed
        }
        }
    eonLATLONG={'lat': lat,'lng': lng}
    pubnub.publish(channel='temperture', message=eonTEMP)
    pubnub.publish(channel='rpm', message=eonRPM)
    pubnub.publish(channel='speed', message=eonSPEED)
    pubnub.publish(channel='my_map', message=eonLATLONG)

    for event in pygame.event.get():
        print event.type
        if event.type==pygame.QUIT:
            pygame.quit()
            quit()
    pygame.display.flip()
    #time.sleep(0.5)
    screen.fill((0,0,0))


#signal.signal(signal.SIGINT, signal_handler)
