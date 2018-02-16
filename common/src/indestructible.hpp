#pragma once

namespace ares {
  template <typename T>
  struct indestructible {

    inline indestructible(const indestructible<T>& other) :
      p_(other.p_) {
    }

    inline indestructible(indestructible<T>&& other) :
      p_(std::move(other.p_)) {
    }

    inline indestructible<T>& operator=(const indestructible<T>& other) {
      p_ = other.p_;
      return *this;
    }

    inline indestructible<T>& operator=(indestructible<T>&& other) {
      p_ = std::move(other.p_);
      return *this;
    }

    inline bool operator==(const indestructible<T>& other) {
      return p_ == other.p_;
    }

    inline bool operator==(const bool& b) {
      return p_ != nullptr;
    }
    
    /*! Returns reference to the contained object */
    inline T& operator*() {
      return *reinterpret_cast<T*>(get());
    }

    inline T* operator->() {
      return reinterpret_cast<T*>(get());
    }

    /*! Calculates the actual pointer for the object in the buffer */
    inline void* get() {
      return (void*)((uintptr_t)p_ + alignof(T) - ((uintptr_t)(p_) % alignof(T)));
    }
    
    /*! Invokes contained object's destructor on demand, but does not delete (deallocate) it */
    void destruct() {
      operator*().~T();
    }

    /*! Deallocates memory for the contained object on demand (without a call to destructor) */
    void free() {
      free(p_);
      p_ = nullptr;
    }
    
    friend struct make_indestructible;
    
  private:
    void* p_;

    inline indestructible() {
      // Use malloc here instead of new, so that the memory could be freed with free() on demand without invoking destructor
      p_ = malloc(sizeof(T) + alignof(T));
      if (p_ == nullptr) throw std::bad_alloc();
    };
  };

  struct make_indestructible {
    /*! Create indestructible by forwarding arguments to underlying constructor */
    template <typename T, typename... Args>
    inline static auto from_constructor(Args&&... args) -> indestructible<T> {
      indestructible<T> i;
      // Invoke constructor with in-place allocated new
      auto obj = new (i.get()) T(std::forward<Args>(args)...);
      if (obj == nullptr) throw std::bad_alloc();  // Just to qwell unused variable warning
      return i;
    }

    /*! Create indestructible by calling copy constructor */
    template <typename T>
    inline static auto from_copy_constructor(T existing) -> indestructible<T> {
      indestructible<T> i;
      // Invoke constructor with in-place allocated new
      auto obj = new (i.get()) T(existing);
      if (obj == nullptr) throw std::bad_alloc();  // Just to qwell unused variable warning
      return i;
    }

    /*! Create indestructible by calling move constructor */
    template <typename T>
    inline static auto from_move_constructor(T&& existing) -> indestructible<T> {
      indestructible<T> i;
      // Invoke constructor with in-place allocated new
      auto obj = new (i.get()) T(std::move(existing));
      if (obj == nullptr) throw std::bad_alloc();  // Just to qwell unused variable warning
      return i;
    }
    
    /*! Create indestructible by calling a factory function. Assumes T is move-constructible */
    template <typename F>
    inline static auto move_from_factory(F factory_function) -> indestructible<decltype(factory_function())> {
      using T = decltype(factory_function());
      indestructible<T> i;
      auto obj = new (i.get()) T(std::move(factory_function()));
      if (obj == nullptr) throw std::bad_alloc();  // Just to qwell unused variable warning
      return i;
    }

    /*! Create indestructible by calling a factory function using copy constructor */
    template <typename F>
    inline static auto copy_from_factory(F factory_function) -> indestructible<decltype(factory_function())> {
      using T = decltype(factory_function());
      indestructible<T> i;
      auto obj = new (i.get()) T(factory_function());
      if (obj == nullptr) throw std::bad_alloc();  // Just to qwell unused variable warning
      return i;
    }
  };
  


}
