#include "texturetextwindow.h"
using namespace std;
using namespace ogl;

TextureTestWindow::TextureTestWindow(int width, int height, const string &filename)
	: mWidth{width}, mHeight{height},
	  mTexture{filename},
	  mPositions{
		  {-1.0f,  1.0f, 0.0f, 1.0f},
		  {-1.0f, -1.0f, 0.0f, 1.0f},
		  { 1.0f,  1.0f, 0.0f, 1.0f},
		  { 1.0f, -1.0f, 0.0f, 1.0f}
		  },
	  mTextCoords{{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}},
	  mCamera{width, height}
{
	createPositionAndTextCoordsBufferObjects();
	createVertexArrayObject();

	mTechnique
			.addShaderProgram(GL_VERTEX_SHADER, {"../resources/vertexshader.glsl"})
			.addShaderProgram(GL_FRAGMENT_SHADER, {"../resources/fragmentshader.glsl"});

	mTechnique.finalize();
	mTechnique.enable();
	mTechnique
			.setUniform(mTechnique.getUniformLocation("tex0"), 0)
			.setUniform(mTechnique.getUniformLocation("model"), glm::mat4());

	glViewport(0, 0, mWidth, mHeight);

	mCamera.setPosition({0.0f, 0.0f, 2.0f})
			.setFieldOfView(45.0f)
			.setNearAndFarPlanes(1.0f, 1000.0f)
			.setViewportAspectRatio((float)mWidth / mHeight);

	updateCamera();
}

void TextureTestWindow::onKeyboardEvent(Key key, KeyState keyState)
{
	const float speed = 0.1f;
	glm::vec3 offsetVec = glm::vec3{0.0f, 0.0f, -1.0f} * speed;

	switch (key) {
	case Key::w:
	case Key::W:
	case Key::UP:
		mCamera.offsetPosition(offsetVec);
		updateCamera();
		break;

	case Key::s:
	case Key::S:
	case Key::DOWN:
		mCamera.offsetPosition(-offsetVec);
		updateCamera();
		break;

	case Key::a:
	case Key::A:
	case Key::LEFT:
		break;

	case Key::d:
	case Key::D:
	case Key::RIGHT:
		break;

	case Key::ESCAPE:
		PostQuitMessage();
	}
}

void TextureTestWindow::onRenderSceneEvent()
{
	glClear(GL_COLOR_BUFFER_BIT);

	mTexture.bind(GL_TEXTURE0);
	enableVertexArrayObject();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, mPositions.size());

	disableVertexes();
}

void TextureTestWindow::onResizeEvent(int width, int height)
{
	mWidth = width;
	mHeight = height;
	glViewport(0, 0, mWidth, mHeight);

	mCamera.setViewportAspectRatio((float)mWidth / mHeight);
	updateCamera();
}

void TextureTestWindow::enableVertexArrayObject()
{
	glBindVertexArray(mArrayBufferObject);
	enableVertexes();
}

void TextureTestWindow::createPositionAndTextCoordsBufferObjects()
{
	// This will identify our vertex buffervoid
	glGenBuffers(1, &mPositionBuffer);
	updatePositionBuffer();

	glGenBuffers(1, &mTextCoords0Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTextCoords0Buffer);
	glBufferData(GL_ARRAY_BUFFER, mTextCoords.size() * sizeof(glm::vec2),
				 mTextCoords.data(), GL_STATIC_DRAW);
}

void TextureTestWindow::mapPositionVertex()
{
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glEnableVertexAttribArray(static_cast<int>(ogl::AttribPosition::position));
	glVertexAttribPointer(static_cast<int>(ogl::AttribPosition::position),
						  4, GL_FLOAT, GL_FALSE,
						  sizeof(mPositions[0]), nullptr);
	glDisableVertexAttribArray(static_cast<int>(ogl::AttribPosition::position));
}

void TextureTestWindow::mapTextCoords0Vertex()
{
	glBindBuffer(GL_ARRAY_BUFFER, mTextCoords0Buffer);
	glEnableVertexAttribArray(static_cast<int>(ogl::AttribPosition::textCoords0));
	glVertexAttribPointer(static_cast<int>(ogl::AttribPosition::textCoords0),
						  2, GL_FLOAT, GL_FALSE,
						  sizeof(mTextCoords[0]), nullptr);
	glDisableVertexAttribArray(static_cast<int>(ogl::AttribPosition::textCoords0));
}

void TextureTestWindow::enableVertexes()
{
	glEnableVertexAttribArray(static_cast<int>(ogl::AttribPosition::position));
	glEnableVertexAttribArray(static_cast<int>(ogl::AttribPosition::textCoords0));
}

void TextureTestWindow::disableVertexes()
{
	glDisableVertexAttribArray(static_cast<int>(ogl::AttribPosition::position));
	glDisableVertexAttribArray(static_cast<int>(ogl::AttribPosition::textCoords0));
}

void TextureTestWindow::updateCamera()
{
	mTechnique.setUniform(mTechnique.getUniformLocation("camera"), mCamera.matrix());
}

void TextureTestWindow::createVertexArrayObject()
{
	glGenVertexArrays(1, &mArrayBufferObject);
	glBindVertexArray(mArrayBufferObject);

	mapPositionVertex();
	mapTextCoords0Vertex();
}

void TextureTestWindow::updatePositionBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(glm::vec4),
				 mPositions.data(), GL_DYNAMIC_DRAW);
}
