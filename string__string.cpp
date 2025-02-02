#include "pch.h"

/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#include <stdarg.h>
#include <math.h>

#include "arch.h"

#include "error/error.h"
#include "file/file.h"

#include "memory/malloc.h"
#include "memory/lalloc.h"
#include "memory/growheap.h"

#include "string/string.h"
#include "string/str_checksum.h"
#include "app/registry.h"

i4_string_manager_class i4_string_man;

i4_string_manager_class::node::node_allocator * i4_string_manager_class::node::nodes=0;
w32 i4_string_manager_class::node::nodes_ref=0;        // number of string managers using 'nodes'

i4_string_manager_class::array_node::node_allocator * i4_string_manager_class::array_node::nodes=0;

static i4_const_str * null_string;

////////////////////////////////////////////////////////////////////////
//
//  Constant String Methods
//

#ifdef _WINDOWS
//use conversion function for current language setting.
#include <windows.h>
i4_char i4_char::to_lower() const
//{ return (ch>='A' && ch<='Z') ? i4_char(ch-'A'+'a') : i4_char(ch); }
{
	return i4_char((w8) CharLower((LPTSTR)ch));
}

i4_char i4_char::to_upper() const
//{ return (ch>='a' && ch<='z') ? i4_char(ch-'a'+'A') : i4_char(ch); }
{
	return i4_char((w8) CharUpper((LPTSTR)ch));
}

#endif

#undef new
//#ifndef i4_NEW_CHECK
void * i4_string_manager_class::node::operator new(size_t size)
{
	return nodes->alloc();
}

void i4_string_manager_class::node::operator delete(void * ptr)
{
	nodes->free((node *)ptr);
}
//#endif


i4_string_manager_class::node::~node()
{
	if (left)
	{
		delete left;
	}

	if (right)
	{
		delete right;
	}
}


//#ifndef i4_NEW_CHECK
void * i4_string_manager_class::array_node::operator new(size_t size)
{
	return nodes->alloc();
}

void i4_string_manager_class::array_node::operator delete(void * ptr)
{
	nodes->free((array_node *)ptr);
}
//#endif


i4_string_manager_class::array_node::~array_node()
{
	if (left)
	{
		delete left;
	}

	if (right)
	{
		delete right;
	}
}

i4_const_str::iterator i4_const_str::strstr(const i4_const_str &needle_to_find) const
{
	iterator j=needle_to_find.begin();

	for (iterator i=begin(); i!=end(); ++i)
	{
		iterator k=i;
		j=needle_to_find.begin();
		while (j.get()==k.get())
		{
			++j;
			if (j==needle_to_find.end())
			{
				return i;
			}
			++k;
		}
	}
	return end();
}
#ifdef _MANAGED
#pragma unmanaged
#endif
i4_str * i4_const_str::vsprintf(w32 max_length, va_list &ap) const
{
	i4_str * ns=new i4_str(*this,(w16)max_length);
	char * fmt=ptr,* out=ns->ptr;
	int charsleft=max_length-1;
	int l=length();

	while (*fmt && l)
	{
		if (*fmt=='%')
		{
			fmt++;
			if (*fmt=='S')
			{
				l--;
				i4_const_str * cs=va_arg(ap, i4_const_str *);
				if (cs)
				{
					i4_const_str::iterator it=cs->begin();
					while (it!=cs->end())
					{
						*out=(char)it.get().value();
						out++;
						charsleft--;
						if (charsleft<=0)
						{
							goto out_of_space;
						}
						++it;
					}
				}
			}
			else
			{
				char fmt_str[10],* fs;
				fs=fmt_str+1;
				fmt_str[0]='%';
				do
				{
					*fs=*fmt;
					fs++;
					fmt++;
					l--;
				}
				while (fmt[-1]!='s' &&
					   fmt[-1]!='d' &&
					   fmt[-1]!='f' &&
					   fmt[-1]!='p' &&
					   fmt[-1]!='g' &&
					   fmt[-1]!='c' &&
					   fmt[-1]!='i' &&
					   fmt[-1]!='x' &&
					   fmt[-1]!='u' &&
					   fmt[-1]!='X' &&
					   fmt[-1]!='o');
				fmt--;
				*fs=0;

				switch (*fmt)
				{
					case 'd':
					case 'i':
					case 'x':
					case 'X':
					case 'o':
						if (charsleft<=20)
						{
							//assume they never take more than this
							goto out_of_space;
						}
							::sprintf(out,fmt_str,va_arg(ap,int));
						break;
					case 'f':
						{
							if (charsleft<=20)
							{
								goto out_of_space;
							}
							float f=(float)va_arg(ap, double);
								::sprintf(out,fmt_str,f);
						} break;
					case 'g':
						if (charsleft<=20)
						{
							goto out_of_space;
						}
							::sprintf(out,fmt_str,va_arg(ap,double));
						break;
					case 's':
						{
							char * s=va_arg(ap,char *);
							int str_l=strlen(s);
							if (charsleft<=str_l)
							{
								goto out_of_space;
							}
								::sprintf(out,fmt_str,s);
							break;
						}
					default:
						if (charsleft<=20)
						{
							goto out_of_space;
						}
							::sprintf(out,fmt_str,va_arg(ap,void *));
						break;
				}
				while (*out)
				{
					out++;
					charsleft--;
					if (charsleft<=0)
					{
						goto out_of_space;
					}
				}
			}

		}
		else
		{
			*out=*fmt;
			out++;
			charsleft--;
		}
		fmt++;
		l--;
	}

	*out=0;

	ns->len=strlen(ns->ptr);
	return ns;

out_of_space:
	*out=0;

	ns->len=strlen(ns->ptr);
	i4_error("CRITICAL: Out of space formating strings. Possible reasons: Internal Bug or Hacker Buffer Overrun Attack");
	return ns;
}

#ifdef _MANAGED
#pragma managed
#endif

i4_str * i4_const_str::sprintf(w32 max_length, ...) const
{


	va_list ap;

	va_start(ap, max_length);

	i4_str * ret=vsprintf(max_length,ap);

	va_end(ap);

	return ret;
}


w32 i4_const_str::iterator::read_ascii(char * buffer, w32 buffer_size)
{
	w32 count=0;

	iterator cur(*this);

	while (cur.get().is_space())
	{
		++cur;
		++node;
	}

	while (buffer_size>0 && cur.get().value() && !cur.get().is_space())
	{
		*(buffer++)=(char)cur.get().value();
		buffer_size--;
		++cur;
		++node;
		++count;
	}
	*buffer=0;
	return count;
}


i4_str * i4_const_str::iterator::read_string()
{
	iterator start(*this);

	while (start.get().value() && start.get().is_space())
	{
		++start;
		++node;
	}

	if (!start.get().value())
	{
		return 0;
	}

	w32 len=0;
	iterator cur(start);

	while (cur.get().value() && !cur.get().is_space())
	{
		++len;
		++cur;
		++node;
	}

	i4_str * ret=new i4_str(start,cur,(w16)len+1);
	return ret;
}


sw32 i4_const_str::iterator::read_number(i4_bool throwexception)
{
	iterator cur(*this);
	while (cur.get().is_space())
	{
		++cur;
		++node;
	}

	sw32 x=0;
	i4_bool error=i4_F;

	i4_bool neg;
	if (cur.get() == i4_char('-'))
	{
		neg=i4_T;
		++cur;
		++node;
	}
	else
	{
		neg=i4_F;
	}


	while (cur.get().value() && !cur.get().is_space() && !error)
	{
		char_type c=(char)cur.get().value();
		if (c<'0' || c>'9')
		{
			error=i4_T;
		}
		else
		{
			x=(x*10)+c-'0';

			++node;
			++cur;
		}
	}

	if (neg)
	{
		x=-x;
	}

	if (error)
	{
		if (throwexception)
		{
			throw "Number format exception";
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return x;
	}
}


double i4_const_str::iterator::read_float(i4_bool throwexception)
{
	while (get().is_space())
		++ (*this);



	double x=0;
	sw32 exp=0;

	i4_bool
	error=i4_F;

	i4_bool neg = i4_F;
	if (get() == i4_char('-'))
	{
		neg=i4_T;
		++ (*this);
	}
	else if (get() == i4_char('+'))
	{
		++ (*this);
	}

	while (get().value())
	{
		if (get()<i4_char('0') || get()>i4_char('9'))
		{
			break;
		}

		x = (x*10) + (double)(get().value()-'0');

		++ (*this);
	}

	if (get() == i4_char('.'))
	{
		++ (*this);
		while (get().value())
		{
			if (get()<i4_char('0') || get()>i4_char('9'))
			{
				break;
			}

			x = (x*10) + (double)(get().value()-'0');
			exp--;

			++ (*this);
		}
	}

	if (get() == i4_char('e') || get() == i4_char('E'))
	{
		++ (*this);
		exp += read_number(throwexception); //throws exception if something odd
	}
	if (get().value())
	{
		if (throwexception)
		{
			throw "Number format exception";
		}
		else
		{
			return 0;
		}
	}

	if (neg)
	{
		x=-x;
	}

	if (error)
	{
		return 0;
	}

	return x *pow(10.0f,(int)exp);
}


////////////////////////////////////////////////////////////////////////
//
//  String methods
//

//WARNING: Maximum buffer size is 64kB. These methods are possibly vulnerable
//to buffer-overrun attacks.


void i4_str::alloc(w16 _buf_len)
{
	buf_len = _buf_len;
	if (buf_len==0)
	{
		ptr=0;
	}
	else
	{
		ptr=(char_type *)I4_MALLOC(sizeof(char_type)*(buf_len+1),"i4_str");
	}
}


void i4_str::init_from_string(const i4_const_str &str, w16 _buf_len)
{
	if (!_buf_len)
	{
		ptr=0;
		return ;
	}

	alloc(_buf_len);

	len = (w16)((str.length() >= buf_len) ? buf_len : str.length());

	i4_const_str::iterator p=str.begin();
	char_type * c=ptr;

	for (w16 x=0 ; x<len; ++x)
	{
		*c = (char_type)( p.get().value() );

		++p;
		++c;
	}
	*c=0;
}

i4_str::i4_str(const char * s)
{
	w16 l=0;

	if (s)
	{
		l=(w16) (strlen(s)>0xfff0 ? 0xfff0 : strlen(s));
	}

	alloc(l);
	if (l)
	{
		memcpy(ptr,s,l+1);
	}
	len=l;
}

i4_str::i4_str(const char c)
{
	len=1;
	alloc(4);
	ptr[0]=c;
	ptr[1]=0;
}

i4_str::i4_str(const i4_const_str::iterator start,
			   const i4_const_str::iterator last, w16 _buf_len)
	: i4_const_str(0)
{
	alloc(_buf_len);

	len=0;

	char_type * c=ptr;
	w16 count=buf_len;
	i4_const_str::iterator s(start);

	while (s!=last && count)
	{
		*c=(char_type)( s.get().value() );
		++c;
		++s;
		++len;
		count--;
	}
	*c=0;
}


void i4_str::insert(i4_str::iterator p, const i4_char ch)
//  insert ch before p
{
	if (len==buf_len&&buf_len<0xfff0) //increase buffer size by 4 characters
	{
		char * nptr=(char *)I4_REALLOC(ptr,sizeof(char_type)*(buf_len+4),"i4_str_bufexpand_char");
		w32 diff=nptr-ptr;
		buf_len=buf_len+4;
		ptr=nptr;
		p.node+=diff;
	}
	else if (buf_len>=0xfff0)
	{
		i4_warning("WARNING: String buffer exceeded maximum size. Character dropped");
		return;
	}
	iterator p2(p);
	if (p2==end())
	{
		p2.set(ch);
		++p2;
		p2.set(0);
		++len;
	}
	else
	{
		iterator c(ptr+len+1);
		*c.node=0;
		while (c!=p2)
		{
			iterator q(c);
			--c;
			q.set(c.get());
			--q;
		}
		c.set(ch);
		++len;
	}

}


void i4_str::insert(i4_str::iterator p, const i4_const_str &other)
// insert other before p, adjust size of this
{
	w32 l=length()+other.length()+1;

	if (l>0xfff0)
	{
		l=0xfff0;
	}                    //Try to avoid vulnerability to buffer-overrun attacks
	if (ptr==0)
	{
		ptr=(char *)I4_MALLOC(other.length()+1,"strbuf");
		ZeroMemory(ptr,other.length()+1);
		strncpy(ptr,other.c_str(),other.length());
		buf_len=other.length()+1;
		len=other.length();
		return;
	} //no here. thats enough.
	if (l>buf_len) //increase buffer size
	{

		char * nptr=(char *)I4_REALLOC(ptr,sizeof(char_type)*(l+2),"i4_str_bufexpand");
		w32 diff=nptr-ptr; //realloc might change ptr,
		//but the iterator still points to the old ptr
		buf_len=(w16)(l+1);
		p.node+=diff;
		ptr=nptr;
	}
	sw16 move_length=(short) ptr_diff(p,end());
	sw16 max_move=(short) (buf_len-(ptr_diff(begin(),p)+other.length()+1));

	if (max_move>0) // see if anything can be moved
	{
		iterator p2(0);
		if (move_length>max_move)
		{
			move_length=max_move;
			p2=iterator(ptr+buf_len-1);
			len=buf_len-1;
		}
		else
		{
			p2=iterator(ptr+ptr_diff(begin(),p)+other.length()+move_length);
			len+=(w16)other.length();
		}

		p2.set(0);
		--p2;

		i4_const_str::iterator p1(p);
		for (w16 x=1; x<move_length; ++x)
		{
			++p1;
		}

		while (move_length)
		{
			p2.set(p1.get());
			--p2;
			--p1;
			move_length--;
		}

	}
	else
	{
		len=buf_len-1;
		ptr[buf_len-1]=0;
	}
	//}}} ??? here end it?
	iterator p3(p);
	// now see how much we can insert
	sw16 insert_len=(short)other.length();
	if (insert_len+ptr_diff(begin(),p3)>=buf_len)
	{
		insert_len=buf_len-ptr_diff(begin(),p3)-1;
	}

	if (insert_len>0) //this is only <=0 if string size exceeds 64k or other.len=0
	{
		i4_const_str::iterator ip=other.begin();
		while (insert_len)
		{
			p3.set(ip.get());
			++p3;
			++ip;
			insert_len--;
		}
	}
}


void i4_str::remove(i4_str::iterator start, i4_str::iterator last)
{
	sw32 rm_len=ptr_diff(last,end());

	w16 cutlen=(w16)ptr_diff(start,last);

	I4_ASSERT(cutlen>=0,"INTERNAL: i4_str::remove() attempt to delete less than 0 bytes");
	len-=cutlen;

	while (rm_len>0)
	{
		start.set(last.get());
		++start;
		++last;
		rm_len--;
	}

	start.set(i4_char(0));
}


void i4_str::to_lower()
{
	for (i4_str::iterator p=begin(); p!=end(); ++p)
	{
		p.set(p.get().to_lower());
	}
}


void i4_str::to_upper()
{
	for (i4_str::iterator p=begin(); p!=end(); ++p)
	{
		p.set(p.get().to_upper());
	}
}

i4_str::~i4_str()
{
	if (ptr)
	{
		i4_free((char *)ptr);
	}
}

int i4_const_str::find(const i4_const_str &needle_to_find) const
{
	iterator j=needle_to_find.begin();
	int pos=0;

	for (iterator i=begin(); i!=end(); ++i)
	{
		iterator k=i;
		j=needle_to_find.begin();
		while (j.get()==k.get())
		{
			++j;
			if (j==needle_to_find.end())
			{
				return pos;
			}
			++k;
		}
		++pos;
	}
	return len;
}
void i4_str::erase(int start, int bytes)
{
	remove(iterator(ptr+start),iterator(ptr+start+bytes));
}

int i4_const_str::find_first_of(const i4_const_str &str) const
{
	i4_char ch;
	w32 j;
	int ip=0;

	for (iterator i=begin(); i!=end(); i++,ip++)
	{
		ch=i.get();
		for(j=0; j<str.length(); j++)
		{
			if(str[j]==ch.ascii_value())
			{
				return ip;
			}
		}
	}
	return -1;
}

int i4_const_str::find_first_not_of(const i4_const_str &str) const
{
	i4_char ch;
	w32 j;
	int ip=0;
	i4_bool found=i4_F;

	for (iterator i=begin(); i!=end(); i++,ip++)
	{
		ch=i.get();
		for(j=0; j<str.length(); j++)
		{
			if(str[j]==ch.ascii_value())
			{
				found=i4_T;
			}
		}
		if (!found)
		{
			return ip;
		}
		found=i4_F;
	}
	return -1;
}

int i4_const_str::find_last_of(const i4_const_str &str) const
{
	i4_char ch;
	w32 j;
	int ip=len-1;
	iterator i=end();

	--i;
	for (; ip>=0; i--,ip--)
	{
		ch=i.get();
		for(j=0; j<str.length(); j++)
		{
			if(str[j]==ch.ascii_value())
			{
				return ip;
			}
		}
	}
	return -1;
}

int i4_const_str::find_last_not_of(const i4_const_str &str) const
{
	i4_char ch;
	w32 j;
	int ip=len-1;
	i4_bool found=i4_F;
	iterator i=end();

	--i;
	for (; ip>=0; i--,ip--)
	{
		ch=i.get();
		for(j=0; j<str.length(); j++)
		{
			if(str[j]==ch.ascii_value())
			{
				found=i4_T;
			}
		}
		if (!found)
		{
			return ip;
		}
		found=i4_F;
	}
	return -1;
}

i4_str i4_str::substr(int start, int &_len) const
{
	i4_str tmp(_len);
	if (start>len)
	{
		_len=0;
		return tmp;
	}
	if (start+_len>len)
	{
		_len=len-start;
	}
	for(int i=start; i<start+_len; i++)
	{
		tmp[i-start]=this->operator[](i);
	}
	tmp.set_length(_len);
	return tmp;
}

////////////////////////////////////////////////////////////////////////
//
// String Manager Methods
//

const i4_const_str &i4_string_manager_class::get(const char * internal_name)
{
	if (!internal_name)
	{
		return * null_string;
	}

	node * n=root;
	while (n)
	{
		int c=strcmp(internal_name,n->str_token);
		if (c<0)
		{
			n=n->left;
		}
		else if (c>0)
		{
			n=n->right;
		}
		else
		{
			return n->value;
		}
	}
	i4_warning("symbol not found %s\n",internal_name);

	return *null_string;
}

void i4_string_manager_class::show_node(node * who)
{
	if (who)
	{
		show_node(who->left);
		i4_warning("%s\n",who->str_token);
		show_node(who->right);
	}
}

void i4_string_manager_class::show_nodes()
{
	if (root)
	{
		show_node(root);
	}
}

i4_const_str * i4_string_manager_class::get_array(const char * internal_name)
{
	array_node * n=array_root;

	while (n)
	{
		int c=strcmp(internal_name,n->str_token);
		if (c<0)
		{
			n=n->left;
		}
		else if (c>0)
		{
			n=n->right;
		}
		else
		{
			w32 count=0,x;
			char * * c=n->value;
			i4_const_str * ret;

			while (*c)
			{
				c++;
				count++;
			}
			ret=(i4_const_str *)I4_MALLOC(sizeof(i4_const_str)*(count+1),"array list");
			for (x=0; x<count; x++)
			{
				ret[x].ptr=n->value[x];
				ret[x].len=strlen(ret[x].ptr);
			}
			ret[count].ptr=0;
			ret[count].len=0;

			return ret;
		}
	}

	i4_error("array not found %s\n",internal_name);
	return 0;
}


void i4_string_manager_class::init()
{
	string_heap=new i4_grow_heap_class(0x1000,0x1000);

	if (!node::nodes)
	{
		node::nodes=new i4_linear_allocator(sizeof(node),0,128,"strings");
		array_node::nodes=new i4_linear_allocator(sizeof(array_node),
												  0,128,"arrays");
		null_string=new i4_const_str(0);
	}
	i4_string_manager_class::node::nodes_ref++;


	root=0;
	array_root=0;
}

i4_string_manager_class::~i4_string_manager_class()
{
	if (string_heap)
	{
		delete string_heap;
	}
}

void i4_string_manager_class::uninit()
{
	if (root)
	{
		delete_node(root);
		root=0;
	}
	if (array_root)
	{
		delete_array_node(array_root);
		array_root=0;
	}

	if (string_heap)
	{
		delete string_heap;
		string_heap=0;
	}

	i4_string_manager_class::node::nodes_ref--;
	if (i4_string_manager_class::node::nodes_ref==0)
	{
		delete node::nodes;       // if there are multiple string managers we can only do this
		delete array_node::nodes; // once, so for now we won't do it
		node::nodes=0;
		delete null_string;
		null_string=0;
	}
}



i4_string_manager_class::i4_string_manager_class()
{
	root=0;
	array_root=0;
	string_heap=0;
}



inline i4_bool space_char(char ch)
{
	return (i4_bool)(ch==' ' || ch=='\n' || ch=='\t' || ch=='\r');
}



char * i4_string_manager_class::alloc_str(char * string)
{
	w16 len=strlen(string)+1;
	void * storage=string_heap->malloc(len,"string");

	memcpy(storage,string,len);
	return (char *)storage;
}



void i4_string_manager_class::add_node(char * token, char * string)
{
	node * * p=&root;

	while (*p)
	{
		if (strcmp(token,(*p)->str_token)<0)
		{
			p=&(*p)->left;
		}
		else
		{
			p=&(*p)->right;
		}
	}
	*p=new_node(alloc_str(token),i4_const_str(alloc_str(string)));
}



void i4_string_manager_class::add_array_node(char * token, char * * array, w32 total)
{
	array_node * * p=&array_root;

	while (*p)
	{
		if (strcmp(token,(*p)->str_token)<0)
		{
			p=&(*p)->left;
		}
		else
		{
			p=&(*p)->right;
		}
	}

	char * * new_array;
	new_array = (char * *)ALIGN_FORWARD( string_heap->malloc((total+1)*sizeof(char *) + sizeof(int) - 1,
															 "string_array") );
	for (int i=0; i<(int)total; i++)
	{
		new_array[i]=array[i];
	}

	new_array[total] = 0;

	*p=new_array_node(alloc_str(token),new_array);
}



static void skip_white(char *&s, w32 &line_on)
{
	char * sl=s; // use local pointer so a register can be substituted

	for(;;)
	{
		switch (*sl)
		{
			case ' ':
			case '\t':
			case '\r':
				sl++;
				break;
			case '\n':
				sl++;
				line_on++;
				break;
			case '/':
				{
					if (sl[1]=='/')
					{
						while (*sl && *sl!='\n' && *sl!='\r')
							sl++;


					}
					else
					{
						s=sl;
						return ;
					}

				} break;
			default:
				s=sl;
				return;
		}
	}
}

static void * dump_file_buf;
static int dump_buf_len;

void i4_string_manager_class::expand_macro(char *&s, char *&buf, w32 &line_on, char * error_prefix)
{
	char * sl=s+1,* start;
	w32 len;

	skip_white(sl,line_on);
	if (*sl!='{')
	{
		i4_file_class * fp=i4_open("dump.txt", I4_WRITE);
		fp->write(dump_file_buf, dump_buf_len);
		delete fp;
		i4_error("%s:%d:expected { after $",error_prefix,line_on);
	}

	sl++;
	start=buf;
	get_token(sl,start,line_on,error_prefix);
	*start=0;

	i4_const_str str=get(buf);
	if (str.null())
	{
		i4_error("%s:%d: macro symbol has no value '%s'",error_prefix, line_on, buf);
	}

	start=str.ptr;
	len=str.length();
	while (len)
	{
		len--;
		*buf=*start;
		start++;
		buf++;
	}

	skip_white(sl,line_on);
	if (*sl!='}')
	{
		i4_error("%s:%d: expected }",error_prefix,line_on);
	}
	sl++;
	s=sl;
}


void i4_string_manager_class::get_char(char *&s, char *&buf, w32 &line_on, char * error_prefix)
{
	if (*s=='$')
	{
		expand_macro(s,buf,line_on,error_prefix);
	}
	else if (*s=='\\')
	{
		s++;
		if (*s=='n')
		{
			*buf='\n';
		}
		else if (*s=='r')
		{
			*buf='\r';
		}
		else if (*s=='t')
		{
			*buf='\t';
		}
		else
		{
			*buf=*s;
		}
		s++;
		buf++;
	}
	else
	{
		if (*s=='\n')
		{
			line_on++;
		}
		*buf=*s;
		buf++;
		s++;
	}
}



void i4_string_manager_class::read_array(char *&s,
										 char * * array,
										 w32 &total,
										 w32 &line_on,
										 char * error_prefix,
										 char * token_buf)
{
	char * t1;

	total=0;
	while (*s)
	{
		t1=token_buf;
		get_token(s,t1,line_on,error_prefix);
		*t1=0;
		if (token_buf[0]=='}' && token_buf[1]==0)
		{
			return ;
		}

		array[total]=alloc_str(token_buf);
		total++;
	}
	i4_error("%s:%d: expecting }",error_prefix,line_on);
}



void i4_string_manager_class::get_token(char *&s, char *&buf, w32 &line_on, char * error_prefix)

{
	char * sl=s;

	skip_white(sl,line_on);
	if (*sl=='"')
	{
		sl++;
		while (*sl && *sl!='"')
			get_char(sl,buf,line_on,error_prefix);


		if (*sl==0)
		{
			i4_error("%s:%d:expected %c",error_prefix,line_on,'"');
		}
		else
		{
			sl++;
		}
	}
	else
	{
		if (*sl=='=' || *sl=='{' || *sl=='}' || *sl=='#')
		{
			*buf=*sl;
			sl++;
			buf++;
		}
		else
		{
			while (*sl &&
				   !space_char(*sl) &&
				   *sl!='=' && *sl!='}' && *sl!='{')
				get_char(sl,buf,line_on,error_prefix);


		}
	}
	s=sl;

}



i4_bool i4_string_manager_class::load_buffer(void * internal_buffer, char * error_prefix)
{
	char * s=(char *)internal_buffer;
	char * token1,* token2,* t1,* t2;
	char * * array;
	w32 line_on=1;

	token1 = (char *)I4_MALLOC(4000,"token1 buf");
	token2 = (char *)I4_MALLOC(4000,"token2 buf");
	array  = (char * *)I4_MALLOC(1000*sizeof(char *),"array buf");


	dump_file_buf=internal_buffer;
	dump_buf_len=strlen((char *)internal_buffer);


	while (*s)
	{
		t1=token1;
		get_token(s,t1,line_on,error_prefix);
		*t1=0;

		if (token1[0]=='=' && token1[1]==0)
		{
			i4_error("%s:%d: expected token not =", error_prefix, line_on);
		}
		else if (token1[0]=='{')
		{
			i4_error("%s:%d: expected token not {", error_prefix, line_on);
		}
		else if (token1[0]=='#')
		{
			t1=token1;
			get_token(s,t1,line_on,error_prefix);
			*t1=0;
			i4_bool expandlang=i4_F;
			if (strcmp(token1,"include"))
			{
				if (strcmp(token1,"includelang")!=0)
				{
					i4_error("%s: line %d: expected #include or #includelang", error_prefix, line_on);
				}
				else
				{
					expandlang=i4_T;
				}
			}

			t1=token1;
			get_token(s,t1,line_on,error_prefix);
			*t1=0;
			if (expandlang)
			{
				i4_str langstr(token1);
				i4_language_extend(langstr,langstr.end());
				langstr.insert(langstr.end(),".res");
				load(langstr);
			}
			else
			{
				load(token1);
			}
		}
		else
		{
			t2=token2;
			get_token(s,t2,line_on,error_prefix);
			*t2=0;
			if (token2[0]=='=' && token2[1]==0) // it's ok to have an equal here
			{
				t2=token2;
				get_token(s,t2,line_on,error_prefix);
				*t2=0;
			}

			if (token2[0]=='{' && token2[1]==0)
			{
				w32 total;
				read_array(s, array, total, line_on, error_prefix, token2);
				add_array_node(token1,array, total);
			}
			else
			{
				//        printf("'%s' : '%s'\n",token1,token2);
				add_node(token1,token2);
			}
		}

		skip_white(s,line_on);
	}

	i4_free(token1);
	i4_free(token2);
	i4_free(array);

	return i4_T;
}


i4_bool i4_string_manager_class::load(const i4_const_str &filename)
{
	return load(filename.ptr);
}


i4_bool i4_string_manager_class::load(char * filename)
{
	i4_file_class * fp;

	i4_const_str tmp(filename);

	fp=i4_open(tmp);

	if (!fp)
	{
		i4_error("FATAL: Resource file %s missing.",filename);
		return i4_F;
	}
	else
	{
		w32 size=fp->size();
		char * mem=(char *)I4_MALLOC(size+1,"tmp");
		if (fp->read(mem,size)!=size)
		{
			delete fp;
			return i4_F;
		}
		mem[size]=0;

		load_buffer(mem,filename);
		i4_free(mem);
		delete fp;
	}
	return i4_T;
}




char *i4_os_string(const i4_const_str &name, char * tmp_buf, int buflen)
{
	i4_const_str::iterator p=name.begin();
	char * s=tmp_buf;

	while (p!=name.end() && buflen>1)
	{
		*s=(char)p.get().value();
		++s;
		++p;
		buflen--;
	}
	*s=0;

	return tmp_buf;
}

i4_str *i4_from_ascii(const char * buf)
{
	int l=strlen(buf);
	i4_str * ret=new i4_str(l);

	ret->len=l;
	memcpy(ret->ptr, buf, l);
	return ret;
}


const i4_const_str &i4gets(char * str, i4_bool barf_on_error)
{
	const i4_const_str * s=&i4_string_man.get(str);

	if (barf_on_error && s->null())
	{
		i4_error("Resource missing %s",str);
	}
	return *s;
}

int i4getn(char * str, i4_bool barf_on_error)
{
	i4_const_str::iterator i=i4gets(str, barf_on_error).begin();

	return i.read_number();
}

// STR CHECKSUM
/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/


w32 i4_str_checksum(const i4_const_str &str)
{
	i4_const_str::iterator i=str.begin();

	int len=str.length();

	w8 c1=0,c2=0,c3=0,c4=0;

	while (len)
	{
		c1+=i.get().value();
		c2+=c1;
		c3+=c2;
		c4+=c3;
		++i;
		len--;
	}

	return (c1|(c2<<8)|(c3<<16)|(c4<<24));
}
