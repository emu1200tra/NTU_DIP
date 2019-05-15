from PIL import Image

rawData = open("./raw/B.raw", 'rb').read()
imgSize = (256,256)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/B.png")

rawData = open("./raw/S.raw", 'rb').read()
imgSize = (256,256)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/S.png")



rawData = open("./raw/k.raw", 'rb').read()
imgSize = (512,512)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/k.png")



rawData = open("./raw/closing.raw", 'rb').read()
imgSize = (256,256)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/closing.png")


rawData = open("./raw/exchange.raw", 'rb').read()
imgSize = (512,512)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/exchange.png")

rawData = open("./raw/test_bound.raw", 'rb').read()
imgSize = (256,256)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/test_bound.png")

rawData = open("./raw/label.raw", 'rb').read()
imgSize = (256,256)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/label.png")

rawData = open("./raw/seq.raw", 'rb').read()
imgSize = (256,256)
img = Image.frombytes('L', imgSize, rawData)
img.save("./raw/seq.png")

