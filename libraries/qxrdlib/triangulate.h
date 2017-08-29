#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
   int p1,p2,p3;
} ITRIANGLE;
typedef struct {
   int p1,p2;
} IEDGE;
typedef struct {
   double x,y,z;
} XYZ;

extern int Triangulate(int nv,XYZ *pxyz,ITRIANGLE *v,int *ntri);

#ifdef __cplusplus
}
#endif

#endif // TRIANGULATE_H
