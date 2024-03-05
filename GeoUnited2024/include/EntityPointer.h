// EntityPointer.h

#ifndef __EntityPointer_h__
#define __EntityPointer_h__

class IgesEntity;

struct EntityPointer 
{
public:
	EntityPointer();
	~EntityPointer();
    long GetpDE();
    void SetpDE ( long newpDE );
    long GetpPE ();
    void SetpPE ( long newpPE );
    IgesEntity * GetEntityPointer();
    void SetEntityPointer( IgesEntity * newp );
	void SetType ( int newtype )
	{
		type = newtype;
	}
	
	int GetType ()
	{
		return type;
	}

private:
	long p_DE; // pointer to begining line of directory entry
	long p_PE; // pointer to begining line of parameter data entry
	IgesEntity * p_entity; 
	int type;
};

#endif // EntityPointer
