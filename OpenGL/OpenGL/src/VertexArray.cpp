#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        unsigned int offset = 0;
        GLCall(glEnableVertexAttribArray(i));
        ///�ڶ������������һ�����͵Ĳ����Ķ������ԣ���������2
        ///����������� stride����ָÿһ�� vertex ��Ҫ���Ŀռ䣬���� position ����float��texture��Ҫ����float��normal��Ҫ����float
        ///������������ pointer����ָÿ�����͵Ķ���ĵ�һ�����Ԫ����������λ�á���һ�����֣�Ҫǿ����ָ�롣
        ///����GPU���ݵ� layout
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normlized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
