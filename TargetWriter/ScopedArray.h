#ifndef _scopedarray__h_
#define _scopedarray__h_

/**
 * @brief Utility class for scoped arrays. Allocate these on the stack. When
 * the instances go out of scope, the data they encapsulate will be cleaned 
 * up.
 *
 * @author Shane Kirk
 * @since 26 February 2013
 * @version 1.0
 */
template<class T>
class ScopedArray 
{
	public:

		/**
		 * @brief Constructor.
		 *
		 * @author Shane Kirk
		 * @since 26 February 2013
		 * @version 1.0
		 */
		ScopedArray(T *pAry) : m_pAry(pAry) {}

		/**
		 * @brief Destructor.
		 *
		 * @author Shane Kirk
		 * @since 26 February 2013
		 * @version 1.0
		 */
		~ScopedArray() { if (m_pAry) delete [] m_pAry; }

		/**
		 * @brief Returns pointer to the array.
		 *
		 * @author Shane Kirk
		 * @since 26 February 2013
		 * @version 1.0
		 */
		T * get() { return m_pAry; }

	private:

		//! Copy constructor. NOT IMPLEMENTED.
		ScopedArray(const ScopedArray &);

		//! Assignment operator. NOT IMPLEMENTED.
		const ScopedArray & operator=(const ScopedArray &);

		T *m_pAry; //! The array to deallocate.
};

#endif
