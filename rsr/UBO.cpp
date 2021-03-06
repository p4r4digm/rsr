#include "UBO.hpp"

#include "GL/glew.h"

class UBO {
   bool m_built = false;
   size_t m_size = 0;
   uintptr_t m_ubo;
   
   void build() {
      glGenBuffers(1, (GLuint*)&m_ubo);

      glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
      glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, GL_STATIC_DRAW);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);

      m_built = true;
   }

public:
   UBO(size_t size):m_size(size) { }
   ~UBO(){
      if (m_built) {
         glDeleteBuffers(1, (GLuint*)&m_ubo);
      }
   }
   void setData(UBO *self, size_t offset, size_t size, void *data) {
      if (!m_built) {
         build();
      }

      glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
      glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
   }

   void bind(UBOSlot slot) {
      if (!m_built) {
         build();
      }

      glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_ubo);
   }
};


UBO *UBOManager::create(size_t size) { return new UBO(size); }
void UBOManager::destroy(UBO *self) { delete self; }

void UBOManager::setData(UBO *self, size_t offset, size_t size, void *data) { self->setData(self, offset, size, data); }
void UBOManager::bind(UBO *self, UBOSlot slot) { self->bind(slot); }
