#ifndef _Texture2D_h_
#define _Texture2D_h_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


class Image
    {
    public:
        Image(void);
        Image(GLsizei width, GLsizei height, void *data=0);
        ~Image(void);
        GLsizei width(void) const { return width_; }
        GLsizei height(void) const { return height_; }
        void *data(void) const { return imagedata_; }
        void setSize(GLsizei width, GLsizei height);
        void setData(void *);
        void allocateData(void);
        void freeData(void);
    private:
        GLsizei width_, height_;
        void * imagedata_;
        bool allocatedData_;
    };

class FileImage : public Image
    {
    public:
        FileImage(const char * filename=0);
        void loadFile(const char * filename);
    private:
        int fileType(const char *filename);
        void loadTiffFile(const char *filename);
//        void loadPngFile(const char *filename);
    };


class Texture
    {
    public:
        Texture(GLint wrap=GL_REPEAT, GLint min=GL_LINEAR, GLint mag=GL_LINEAR);
        virtual ~Texture(void);
        virtual void apply(void) = 0;
        virtual void disable(void) = 0;

        void setMinFilter(GLint);
        void setMagFilter(GLint);
        void setWrapS(GLint);
        void setWrapT(GLint);
        void setWrapR(GLint);
        void setWrap(GLint v) { setWrapS(v); setWrapT(v); setWrapR(v); }
        GLint minFilter(void) const { return minFilter_; }
        GLint magFilter(void) const { return magFilter_; }
        GLint wrapS(void) const { return wrapS_; }
        GLint wrapT(void) const { return wrapT_; }
        GLint wrapR(void) const { return wrapR_; }

    private:
        GLint minFilter_, magFilter_, wrapS_, wrapT_, wrapR_;
    };


class Texture2D : public Texture
    {
    public:
        Texture2D(const char *filename, GLint wrap=GL_REPEAT, GLint min=GL_LINEAR, GLint mag=GL_LINEAR);
        Texture2D(const char *filename, const char *alphafilename, GLint wrap=GL_REPEAT, GLint min=GL_LINEAR, GLint mag=GL_LINEAR);
        Texture2D(Image& image, GLint wrap=GL_REPEAT, GLint min=GL_LINEAR, GLint mag=GL_LINEAR);
        virtual ~Texture2D(void);
        virtual void apply(void);
        virtual void disable(void);
        void define(void);
        void subload(GLint xoffset=0,GLint yoffset=0);
        
        void setImage(Image&);
        Image& image(void) const { return *image_; }
        static Texture2D * loadTexture(const char *filename,bool mipmap);
        static Texture2D * loadTexture(const char *filename,const char *afilename,bool mipmap);
        static void initTextures(void);
    private:
        Image * image_;
        GLuint id_;
        void *rescaledImageData_;
        bool defined_;
        bool createdImage_;
    };



#endif
