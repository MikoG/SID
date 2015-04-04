/* 
 * File:   frustum.cpp
 * Author: geth2k
 * 
 * Created on 25 May 2014, 14:19
 */

#include "SID_frustum.h"

sid::frustum::frustum() {
   std::array<float, 16> proj;
   std::array<float, 16> modl;
   std::array<float, 16> clip;
   float t;

   /* Get the current PROJECTION matrix from OpenGL */
   glGetFloatv(GL_PROJECTION_MATRIX, proj.data());

   /* Get the current MODELVIEW matrix from OpenGL */
   glGetFloatv(GL_MODELVIEW_MATRIX, modl.data());

   /* Combine the two matrices (multiply projection by modelview) */
   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

   /* Extract the numbers for the RIGHT plane */
   m_vfFrustum[0][0] = clip[ 3] - clip[ 0];
   m_vfFrustum[0][1] = clip[ 7] - clip[ 4];
   m_vfFrustum[0][2] = clip[11] - clip[ 8];
   m_vfFrustum[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( m_vfFrustum[0][0] * m_vfFrustum[0][0] + m_vfFrustum[0][1] * m_vfFrustum[0][1] + m_vfFrustum[0][2] * m_vfFrustum[0][2] );
   m_vfFrustum[0][0] /= t;
   m_vfFrustum[0][1] /= t;
   m_vfFrustum[0][2] /= t;
   m_vfFrustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   m_vfFrustum[1][0] = clip[ 3] + clip[ 0];
   m_vfFrustum[1][1] = clip[ 7] + clip[ 4];
   m_vfFrustum[1][2] = clip[11] + clip[ 8];
   m_vfFrustum[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( m_vfFrustum[1][0] * m_vfFrustum[1][0] + m_vfFrustum[1][1] * m_vfFrustum[1][1] + m_vfFrustum[1][2] * m_vfFrustum[1][2] );
   m_vfFrustum[1][0] /= t;
   m_vfFrustum[1][1] /= t;
   m_vfFrustum[1][2] /= t;
   m_vfFrustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   m_vfFrustum[2][0] = clip[ 3] + clip[ 1];
   m_vfFrustum[2][1] = clip[ 7] + clip[ 5];
   m_vfFrustum[2][2] = clip[11] + clip[ 9];
   m_vfFrustum[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( m_vfFrustum[2][0] * m_vfFrustum[2][0] + m_vfFrustum[2][1] * m_vfFrustum[2][1] + m_vfFrustum[2][2] * m_vfFrustum[2][2] );
   m_vfFrustum[2][0] /= t;
   m_vfFrustum[2][1] /= t;
   m_vfFrustum[2][2] /= t;
   m_vfFrustum[2][3] /= t;

   /* Extract the TOP plane */
   m_vfFrustum[3][0] = clip[ 3] - clip[ 1];
   m_vfFrustum[3][1] = clip[ 7] - clip[ 5];
   m_vfFrustum[3][2] = clip[11] - clip[ 9];
   m_vfFrustum[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( m_vfFrustum[3][0] * m_vfFrustum[3][0] + m_vfFrustum[3][1] * m_vfFrustum[3][1] + m_vfFrustum[3][2] * m_vfFrustum[3][2] );
   m_vfFrustum[3][0] /= t;
   m_vfFrustum[3][1] /= t;
   m_vfFrustum[3][2] /= t;
   m_vfFrustum[3][3] /= t;

   /* Extract the FAR plane */
   m_vfFrustum[4][0] = clip[ 3] - clip[ 2];
   m_vfFrustum[4][1] = clip[ 7] - clip[ 6];
   m_vfFrustum[4][2] = clip[11] - clip[10];
   m_vfFrustum[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( m_vfFrustum[4][0] * m_vfFrustum[4][0] + m_vfFrustum[4][1] * m_vfFrustum[4][1] + m_vfFrustum[4][2] * m_vfFrustum[4][2] );
   m_vfFrustum[4][0] /= t;
   m_vfFrustum[4][1] /= t;
   m_vfFrustum[4][2] /= t;
   m_vfFrustum[4][3] /= t;

   /* Extract the NEAR plane */
   m_vfFrustum[5][0] = clip[ 3] + clip[ 2];
   m_vfFrustum[5][1] = clip[ 7] + clip[ 6];
   m_vfFrustum[5][2] = clip[11] + clip[10];
   m_vfFrustum[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( m_vfFrustum[5][0] * m_vfFrustum[5][0] + m_vfFrustum[5][1] * m_vfFrustum[5][1] + m_vfFrustum[5][2] * m_vfFrustum[5][2] );
   m_vfFrustum[5][0] /= t;
   m_vfFrustum[5][1] /= t;
   m_vfFrustum[5][2] /= t;
   m_vfFrustum[5][3] /= t;    
}

sid::frustum::frustum(const frustum& orig) {
}

sid::frustum::~frustum() {
}

bool sid::frustum::testPoint(float x, float y, float z) const {

   for(int p = 0; p < 6; p++ )
      if(m_vfFrustum[p][0] * x + 
              m_vfFrustum[p][1] * y + 
              m_vfFrustum[p][2] * z + 
              m_vfFrustum[p][3] <= -50 )
         return false;
   return true;
}