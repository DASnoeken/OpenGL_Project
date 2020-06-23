#pragma once

class IndexBuffer {
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline unsigned int getCount() const { return m_Count; }
private:
	unsigned int m_Count;
	unsigned int m_RendererID;
};