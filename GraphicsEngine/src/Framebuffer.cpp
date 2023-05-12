#include "Framebuffer.h"

#include "glad/glad.h"

void Framebuffer::Bind()
{
	// All following read and write framebuffer operations will affect the currently bound framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Generate()
{
	glGenFramebuffers(1, &handle);
}

void Framebuffer::Delete()
{
	glDeleteFramebuffers(1, &handle);
}

void Framebuffer::CheckStatus()
{
	GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		ELOG("Error generating a framebuffer\n");
		switch (framebufferStatus)
		{
		case GL_FRAMEBUFFER_UNDEFINED:						ELOG("GL_FRAMEBUFFER_UNDEFINED\n");						break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			ELOG("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	ELOG("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"); break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			ELOG("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");		break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			ELOG("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");		break;
		case GL_FRAMEBUFFER_UNSUPPORTED:					ELOG("GL_FRAMEBUFFER_UNSUPPORTED\n");					break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			ELOG("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");		break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		ELOG("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");		break;
		default:											ELOG("Unknown framebuffer status error\n");				break;
		}
	}
}

void Framebuffer::AttachTexture(FBAttachmentType attachmentType, glm::ivec2& size)
{
	if (attachmentType == FBAttachmentType::DEPTH)
		u32 depthTextureHandle = CreateAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, size); // For OpenGL to write the values

	u32 textureHandle = CreateAttachment(GL_COLOR_ATTACHMENT0 + colorAttachmentHandles.size(), GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, size); // For us to write & read the values
	colorAttachmentHandles.push_back(textureHandle);
}

u32 Framebuffer::CreateAttachment(GLenum attachmentType, GLint internalFormat, GLenum dataFormat, GLenum dataType, glm::ivec2& size)
{
	GLuint attachmentHandle;
	glGenTextures(1, &attachmentHandle);
	glBindTexture(GL_TEXTURE_2D, attachmentHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, dataFormat, dataType, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sizeX, sizeY, 0, GL_RGBA8, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, sizeX, sizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

	/*Shouldnt be necessary to set wrapping methods*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, attachmentHandle, 0);

	return attachmentHandle;
}

void Framebuffer::SetBuffers()
{
	std::vector<GLuint> buffers;
	buffers.reserve(colorAttachmentHandles.size());

	for (u32 i = 0; i < colorAttachmentHandles.size(); ++i)
		buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);

	glDrawBuffers(colorAttachmentHandles.size(), buffers.data());
}