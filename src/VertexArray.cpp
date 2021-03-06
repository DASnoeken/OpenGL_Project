#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include <iostream>

VertexArray::VertexArray(){
	GLCALL(glGenVertexArrays(1, &m_RendererID));
	GLCALL(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray(){
	GLCALL(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
	Bind();
	vb.Bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size();i++) {
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::Bind() const{
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const{
	GLCALL(glBindVertexArray(0));
}
