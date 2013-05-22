#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tiffio.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "Texture2D.h"


static void checkError(const char *s)
    {
    GLenum errorNum = glGetError();
    const char * error;
    if (errorNum == GL_NO_ERROR)
        return;
    if (errorNum == GL_INVALID_ENUM)
        error = "GL_INVALID_ENUM";
    else if (errorNum == GL_INVALID_VALUE)
        error = "GL_INVALID_VALUE";
    else if (errorNum == GL_INVALID_OPERATION)
        error = "GL_INVALID_OPERATION";
    else if (errorNum == GL_INVALID_FRAMEBUFFER_OPERATION)
        error = "GL_INVALID_FRAMEBUFFER_OPERATION";
    else if (errorNum == GL_OUT_OF_MEMORY)
        error = "GL_OUT_OF_MEMORY";
    fprintf(stderr,"%s: error %s\n",s,error);
    }




Image::Image(void)
    {
    width_ = height_ = 0;
    imagedata_ = 0;
    allocatedData_ = false;
    }


Image::Image(GLsizei width, GLsizei height, void *data)
    {
    width_ = width;
    height_ = height;
    imagedata_ = data;
    allocatedData_ = false;
    if (!imagedata_)
        allocateData();
    }


Image::~Image(void)
    {
    if (allocatedData_)
        freeData();
    }


void Image::setSize(GLsizei width, GLsizei height)
    {
    width_ = width;
    height_ = height;
    }


void Image::setData(void *data)
    {
    imagedata_ = data;
    allocatedData_ = false;
    }


void Image::allocateData(void)
    {
    imagedata_ = calloc(1, width_ * height_ * sizeof(unsigned long));
    if (!imagedata_)
        perror("Image::allocateData(): calloc()");
    else
        allocatedData_ = true;
    }


void Image::freeData(void)
    {
    if (imagedata_)
        free(imagedata_);
    imagedata_ = 0;
    allocatedData_ = false;
    }


#define FILETYPE_UNKNOWN    0
#define FILETYPE_TIFF   1
#define FILETYPE_PNG   2

FileImage::FileImage(const char *filename)
    {
    if (filename)
        loadFile(filename);
    }


void FileImage::loadFile(const char *filename)
    {
    int filetype = fileType(filename);
    if (filetype == FILETYPE_UNKNOWN)
        fprintf(stderr,"FileImage::loadFile: image file '%s' is of unknown type\n",filename);
    else if (filetype == FILETYPE_TIFF)
        loadTiffFile(filename);
/*
    else if (filetype == FILETYPE_PNG)
        loadPngFile(filename);
*/
    }


int FileImage::fileType(const char *filename)
    {
    static struct { const char *ext; int typ; } table[] =
            { { ".tif", FILETYPE_TIFF },
              { ".tiff", FILETYPE_TIFF },
              { ".png", FILETYPE_PNG },
              { NULL, FILETYPE_UNKNOWN }
            };
    char *extension = strrchr((char*)filename,'.');
    if (extension)
        {
        for (int i=0; table[i].ext; i++)
            {
            if (strcasecmp(extension, table[i].ext) == 0)
                return table[i].typ;
            }
        }
    return FILETYPE_UNKNOWN;
    }


static uint32 switch_endian(uint32 val)
    {
    return ((val&0xff) << 24) | ((val&0xff00) << 8) | ((val&0xff0000) >> 8)
           | ((val&0xff000000) >> 24);
    }


void FileImage::loadTiffFile(const char *filename)
    {
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif)
        {
        uint32 * data;
        uint32 width, height;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        data = (uint32 *) _TIFFmalloc(width * height * sizeof(uint32));
        if ((data) &&
            (TIFFReadRGBAImage(tif, width, height, data, 0)))
                {
#ifdef POWERPC
#ifdef __APPLE__
                for (int i=0; i < width*height; i++)
                    data[i] = switch_endian(data[i]);
#endif
#endif
/* Note: this assumes that any previously loaded image was also a TIFF */
/* (not really a sound assumption - could be improved) */
                if (this->data())
                    _TIFFfree(this->data());
                setSize(width, height);
                setData(data);
                }
        else
            {
            if (data)
                _TIFFfree(data);
            }
        TIFFClose(tif);
        }
    }

/*
#include <png.h>

void FileImage::loadPngFile(const char *filename)
    {
    FILE *fp = fopen(filename,"r");
    if (!fp) { perror(filename); exit(-1); }
    png_structp png_ptr;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    fclose(fp);
    }
*/


Texture::Texture(GLint wrap, GLint min, GLint mag)
    {
    setMinFilter(min);
    setMagFilter(mag);
    setWrap(wrap);
    }


Texture::~Texture(void)
    {
    }


void Texture::disable(void)
    {
    }


void Texture::setMinFilter(GLint v)
    {
    minFilter_ = v;
    }


void Texture::setMagFilter(GLint v)
    {
    magFilter_ = v;
    }


void Texture::setWrapS(GLint v)
    {
    wrapS_ = v;
    }


void Texture::setWrapT(GLint v)
    {
    wrapT_ = v;
    }


void Texture::setWrapR(GLint v)
    {
    wrapR_ = v;
    }


Texture2D::Texture2D(const char *filename, GLint wrap, GLint min, GLint mag) :
                Texture(wrap,min,mag)
    {
    id_ = 0;
    image_ = 0;
    rescaledImageData_ = 0;
    defined_ = false;
    createdImage_ = false;
    if (filename)
        {
        image_ = new FileImage(filename);
        createdImage_ = true;
        }
    }

Texture2D::Texture2D(const char *filename,const char *alphafilename, GLint wrap, GLint min, GLint mag) :
                Texture(wrap,min,mag)
    {
    id_ = 0;
    image_ = 0;
    rescaledImageData_ = 0;
    defined_ = false;
    createdImage_ = false;
    if (filename)
        {
        image_ = new FileImage(filename);
        Image * alphaimage = new FileImage(alphafilename);
        uint32 *data = (uint32*)image_->data();
        uint32 *alphadata = (uint32*)alphaimage->data();
        int size = image_->width()*image_->height();
        for (int i=0; i<size; i++)
            data[i] = (data[i] & 0x00ffffff) | (alphadata[i]&0xff) << 24;
        createdImage_ = true;
        }
    }


Texture2D::Texture2D(Image& image, GLint wrap, GLint min, GLint mag) :
                Texture(wrap,min,mag)
    {
    id_ = 0;
    rescaledImageData_ = 0;
    defined_ = false;
    createdImage_ = false;
    setImage(image);
    }


Texture2D::~Texture2D(void)
    {
    if (rescaledImageData_)
        ::free(rescaledImageData_);
    if (createdImage_)
        delete image_;
    }


void Texture2D::define(void)
    {
    GLsizei xdim2,ydim2;
    void *imgp;
    defined_ = true;
    if ((!image_) || (!image_->data()))
        return;
    xdim2 = 1;
    while (xdim2 <= image_->width())
        xdim2 *= 2;
    xdim2 /= 2;
    ydim2 = 1;
    while (ydim2 <= image_->height())
        ydim2 *= 2;
    ydim2 /= 2;
/*
    if ((image_->width() != xdim2) || (image_->height() != ydim2))
        {
        rescaledImageData_ = (unsigned long *) ::realloc(rescaledImageData_,
                                     xdim2 * ydim2 * sizeof(unsigned long));
        gluScaleImage(GL_RGBA, image_->width(), image_->height(),
                      GL_UNSIGNED_BYTE, image_->data(), xdim2, ydim2,
                      GL_UNSIGNED_BYTE, rescaledImageData_);
        imgp = rescaledImageData_;
        }
    else
*/
        imgp = image_->data();
    if (id_ == 0)
        glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
/*
    if ((minFilter() == GL_NEAREST_MIPMAP_NEAREST) ||
          (minFilter() == GL_NEAREST_MIPMAP_LINEAR) ||
          (minFilter() == GL_LINEAR_MIPMAP_NEAREST) ||
          (minFilter() == GL_LINEAR_MIPMAP_LINEAR))
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, xdim2, ydim2, GL_RGBA,
                          GL_UNSIGNED_BYTE, imgp);
    else
*/
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xdim2, ydim2, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, imgp);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magFilter());
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minFilter());
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapS());
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapT());
    glBindTexture(GL_TEXTURE_2D,0);
    }


void Texture2D::subload(GLint xoffset,GLint yoffset)
    {
    if (!defined_)
        return;
    if ((!image_) || (!image_->data()))
        return;
    glBindTexture(GL_TEXTURE_2D, id_);
    if ((minFilter() == GL_NEAREST_MIPMAP_NEAREST) ||
          (minFilter() == GL_NEAREST_MIPMAP_LINEAR) ||
          (minFilter() == GL_LINEAR_MIPMAP_NEAREST) ||
          (minFilter() == GL_LINEAR_MIPMAP_LINEAR))
        printf("ERROR: Texture2D:subload(): subloading a mipmapped texture"
               " is not supported\n");
    else
        glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset,
                        image_->width(), image_->height(),
                        GL_RGBA, GL_UNSIGNED_BYTE, image_->data());
    glBindTexture(GL_TEXTURE_2D,0);
    }


void Texture2D::apply(void)
    {
    if (!defined_)
        define();
    if (id_)
        {
        glBindTexture(GL_TEXTURE_2D,id_);
//        glEnable(GL_TEXTURE_2D);     /* Not needed in OpenGL 3 (generates error) */
        }
    else
        {
        glBindTexture(GL_TEXTURE_2D,0);
//        glDisable(GL_TEXTURE_2D);     /* Not needed in OpenGL 3 (generates error) */
        }
    }


void Texture2D::disable(void)
    {
    glBindTexture(GL_TEXTURE_2D,0);
//    glDisable(GL_TEXTURE_2D);     /* Not needed in OpenGL 3 (generates error) */
    }


void Texture2D::setImage(Image& im)
    {
    image_ = &im;
    }



#define MAX_TEXTURES 1000
static struct { char *filename; Texture2D * tex; } loadedTextures[MAX_TEXTURES];
static int numLoadedTextures = 0;

Texture2D * Texture2D::loadTexture(const char *filename,bool mipmap)
    {
    for (int i=0; i < numLoadedTextures; i++)
        if (!strcmp(filename,loadedTextures[i].filename))
            return loadedTextures[i].tex;
    printf("loading %s\n",filename);
    Texture2D *tex;
    if (mipmap)
        tex = new Texture2D(filename,GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR);
    else
        tex = new Texture2D(filename);
    if (numLoadedTextures < MAX_TEXTURES)
        {
        loadedTextures[numLoadedTextures].filename = strdup(filename);
        loadedTextures[numLoadedTextures].tex = tex;
        numLoadedTextures++;
        }
    return tex;
    }

Texture2D * Texture2D::loadTexture(const char *filename,const char *afilename,bool mipmap)
    {
    for (int i=0; i < numLoadedTextures; i++)
        if (!strcmp(filename,loadedTextures[i].filename))
            return loadedTextures[i].tex;
    printf("loading %s\n",filename);
    Texture2D *tex;
    if (mipmap)
        tex = new Texture2D(filename,afilename,GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR);
    else
        tex = new Texture2D(filename,afilename);
    if (numLoadedTextures < MAX_TEXTURES)
        {
        loadedTextures[numLoadedTextures].filename = strdup(filename);
        loadedTextures[numLoadedTextures].tex = tex;
        numLoadedTextures++;
        }
    return tex;
    }

void Texture2D::initTextures(void)
    {
    for (int i=0; i < numLoadedTextures; i++)
        loadedTextures[i].tex->apply();
    }
