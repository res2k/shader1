/**\file
 * Object base class. Reference-countable with boost::intrusive_ptr<>.
 */
#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

namespace s1
{
  class Object
  {
  public:
    Object() : refCount (0) {}
    virtual ~Object () {}

    /// Add a reference to the object
    void AddRef ()
    {
      refCount++;
    }
    /// Release a reference to the object
    void Release ()
    {
      if (--refCount == 0)
        delete this;
    }
  private:
    int refCount;
  };


  static inline void intrusive_ptr_add_ref (Object* p)
  {
    p->AddRef();
  }

  static inline void intrusive_ptr_release (Object* p)
  {
    p->Release();
  }
} // namespace s1

#endif // __BASE_OBJECT_H__
