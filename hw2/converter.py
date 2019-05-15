from PIL import Image

rawData = open("../raw/color_result.raw", 'rb').read()
imgSize = (512,512)
img = Image.frombytes('L', imgSize, rawData)
img.save("../raw/color_result.png")


'''
rawData = open("../raw/test_threshold.raw", 'rb').read()
imgSize = (512,512)
img = Image.frombytes('L', imgSize, rawData)
img.save("../raw/test_threshold.png")

rawData = open("../raw/contrast.raw", 'rb').read()
imgSize = (512,512)
img = Image.frombytes('L', imgSize, rawData)
img.save("../raw/contrast.png")
'''
rawData = open("../raw/color_result2.raw", 'rb').read()
imgSize = (512,512)
img = Image.frombytes('L', imgSize, rawData)
img.save("../raw/color_result2.png")
