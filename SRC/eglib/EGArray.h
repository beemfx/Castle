// EGArray - A templated array that resizes as nesecary.
// (c) 2015 - Beem Software
#pragma once

template<class T> class EGArray
{
public:
	EGArray()
		: m_A(null)
		, m_ASize(0)
		, m_Length(0)
	{ 

	}

	~EGArray()
	{
		if( m_A )
		{
			delete [] m_A;
		}
	}

	void Reserve( eg_size_t Size )
	{
		if( Size > m_ASize )
		{
			Resize( Size );
		}
	}

	void Resize( eg_size_t Size )
	{
		if( Size == m_ASize )
		{
			return;
		}

		if( null == m_A )
		{
			assert( 0 == m_ASize );
			assert( 0 == m_Length );
			m_A = new T[Size];
			m_ASize = Size;
		}
		else
		{
			T* NewA = new T[Size];
			if( null != NewA )
			{
				m_ASize = Size;
				m_Length = EG_Min( m_Length , m_ASize ); //Possibly shrink the array.

																	  // Use = operator to make sure copy happens correctly.
				for( eg_size_t i=0; i<m_Length; i++ )
				{
					NewA[i] = m_A[i];
				}
				delete [] m_A;
				m_A = NewA;
			}
			else
			{
				assert( false ); //Couldn't resize array.
			}
		}
	}

	const T& operator[]( eg_size_t Index )const
	{
		assert( Index < m_ASize ); //Nothing to return! Crash likely.
		return m_A[Index];
	}

	T& operator[]( eg_size_t Index )
	{
		if( Index >= m_ASize )
		{
			Resize( EG_Max(Index+1,m_ASize*2) );
		}

		if( Index >= m_Length )
		{
			m_Length = Index+1;
		}

		assert( Index < m_ASize ); //Nothing to return! Crash likely (Ran out of memory in Resize?)
		return m_A[Index];
	}

	eg_size_t GetLength()const{ return m_Length; }
	void Append( const T& NewItem )
	{
		(*this)[GetLength()] = NewItem;
	}

protected:
	T*        m_A;
	eg_size_t m_ASize;  //Number of reserved elements
	eg_size_t m_Length; //Actual length of the array.
};
