/*
Copyright (c) 2013, Erick<RetroZelda>Folckemer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

using System;
using Sce.PlayStation.Core;
using Sce.PlayStation.Core.Environment;
using Sce.PlayStation.Core.Graphics;
using Sce.PlayStation.Core.Imaging;

namespace VitaRemoteClient
{
	public static class Draw
	{
		private static GraphicsContext graphics;
		private static ShaderProgram shaderProgram;
		private static float[] texcoords = {
			0.0f, 0.0f,	// 0 top left.
			0.0f, 1.0f,	// 1 bottom left.
			1.0f, 0.0f,	// 2 top right.
			1.0f, 1.0f,	// 3 bottom right.
		};
		
		
		private  const int indexSize = 4;
		private static ushort[] indices;

		private static VertexBuffer vertexBuffer0;
		private static VertexBuffer vertexBuffer1;
		private static VertexBuffer vertexBuffer2;
		
		private static float Width;
		
		private static float Height;
		
		private static Matrix4 unitScreenMatrix;
		
		private static Texture2D textTexture;
		
		public static void init(GraphicsContext graphicContext)
		{
			graphics = graphicContext;
			shaderProgram = new ShaderProgram("/Application/shaders/Texture.cgx");
			shaderProgram.SetUniformBinding(0, "WorldViewProj");
      	  	shaderProgram.SetAttributeBinding(0, "a_Position");
       		shaderProgram.SetAttributeBinding(1, "a_TexCoord");
			
			textTexture = new Texture2D("/Application/test2.png", false);
			
			ImageRect rectScreen = graphics.Screen.Rectangle;
			Width = rectScreen.Width;
			Height = rectScreen.Height;
			
			// build index list
			indices = new ushort[indexSize];
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 3;
			
			// build buffers
			vertexBuffer0 = new VertexBuffer(4, indexSize, VertexFormat.Float3, VertexFormat.Float2);
			vertexBuffer1 = new VertexBuffer(4, indexSize, VertexFormat.Float3, VertexFormat.Float2);
			vertexBuffer2 = new VertexBuffer(4, indexSize, VertexFormat.Float3, VertexFormat.Float2);
			
			// build verts for the full quad
			float[] vertices =new float[12];
			vertices[0]=0.0f;	// x0
			vertices[1]=0.0f;	// y0
			vertices[2]=0.0f;	// z0
			
			vertices[3]=0.0f;	// x1
			vertices[4]=1.0f;	// y1
			vertices[5]=0.0f;	// z1
			
			vertices[6]=1.0f;	// x2
			vertices[7]=0.0f;	// y2
			vertices[8]=0.0f;	// z2
			
			vertices[9]=1.0f;	// x3
			vertices[10]=1.0f;	// y3
			vertices[11]=0.0f;	// z3
			
			// build buffer0
			vertexBuffer0.SetVertices(0, vertices);
			vertexBuffer0.SetVertices(1, texcoords);			
			vertexBuffer0.SetIndices(indices);
			
			// build verts for the left quad
			vertices[0]=0.0f;	// x0
			vertices[1]=0.0f;	// y0
			vertices[2]=0.0f;	// z0
			
			vertices[3]=0.0f;	// x1
			vertices[4]=1.0f;	// y1
			vertices[5]=0.0f;	// z1
			
			vertices[6]=0.5f;	// x2
			vertices[7]=0.0f;	// y2
			vertices[8]=0.0f;	// z2
			
			vertices[9]=0.5f;	// x3
			vertices[10]=1.0f;	// y3
			vertices[11]=0.0f;	// z3
			
			// build buffer1
			vertexBuffer1.SetVertices(0, vertices);
			vertexBuffer1.SetVertices(1, texcoords);			
			vertexBuffer1.SetIndices(indices);
			
			
			// build verts for the right quad
			vertices[0]=0.5f;	// x0
			vertices[1]=0.0f;	// y0
			vertices[2]=0.0f;	// z0
			
			vertices[3]=0.5f;	// x1
			vertices[4]=1.0f;	// y1
			vertices[5]=0.0f;	// z1
			
			vertices[6]=1.0f;	// x2
			vertices[7]=0.0f;	// y2
			vertices[8]=0.0f;	// z2
			
			vertices[9]=1.0f;	// x3
			vertices[10]=1.0f;	// y3
			vertices[11]=0.0f;	// z3
			
			// build buffer2
			vertexBuffer2.SetVertices(0, vertices);
			vertexBuffer2.SetVertices(1, texcoords);			
			vertexBuffer2.SetIndices(indices);
			
			unitScreenMatrix = new Matrix4
				( Width*2.0f/rectScreen.Width, 0.0f, 0.0f, 0.0f,
				 0.0f, Height*(-2.0f)/rectScreen.Height, 0.0f, 0.0f,		
				 0.0f, 0.0f, 1.0f, 0.0f,
				 -1.0f, 1.0f, 0.0f, 1.0f
			);
		}
		
		public static void render(Texture2D texture0)
		{
			shaderProgram.SetUniformValue(0, ref unitScreenMatrix);
			graphics.SetShaderProgram(shaderProgram);
			
			// draw the full screen
			graphics.SetVertexBuffer(0, vertexBuffer0);
			graphics.SetTexture(0, texture0);			
			graphics.DrawArrays(DrawMode.TriangleStrip, 0, indexSize);
			
		}
		public static void renderTest()
		{
			shaderProgram.SetUniformValue(0, ref unitScreenMatrix);
			graphics.SetShaderProgram(shaderProgram);
			
			// draw the full screen
			graphics.SetVertexBuffer(0, vertexBuffer0);
			graphics.SetTexture(0, textTexture);			
			graphics.DrawArrays(DrawMode.TriangleStrip, 0, indexSize);
			
		}
		public static void render(Texture2D texture1, Texture2D texture2)
		{
			shaderProgram.SetUniformValue(0, ref unitScreenMatrix);
			graphics.SetShaderProgram(shaderProgram);
			
			// draw the left side
			graphics.SetVertexBuffer(0, vertexBuffer1);
			graphics.SetTexture(0, texture1);			
			graphics.DrawArrays(DrawMode.TriangleStrip, 0, indexSize);
			
			// draw the right side
			graphics.SetVertexBuffer(0, vertexBuffer2);
			graphics.SetTexture(0, texture2);			
			graphics.DrawArrays(DrawMode.TriangleStrip, 0, indexSize);
		}
		
	}
}

