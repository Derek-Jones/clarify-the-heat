
# show.R, 21 Jul 15

im = file("~/clarify-the-heat/image_out.bin", "rb")
im_d=readBin(im, integer(), n=1e9, size= 2, endian = "little")
im1=matrix(im_d, ncol =2*320, nrow=2*240)

# Rotate, to undo original camera orientation when phto taken
# image(t(im1), xaxt="n", yaxt="n", col=rainbow(30))

im2=im1
im2[im2==0]=15000
image(t(im2), xaxt="n", yaxt="n", col=rainbow(30))

