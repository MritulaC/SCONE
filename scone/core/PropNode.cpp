#include "stdafx.h"

#include "PropNode.h"

#include <set>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "boost/foreach.hpp"

using namespace boost;
using namespace boost::property_tree;

namespace scone
{
	PropNode::PropNode()
	{
	}

	PropNode::PropNode( const char* prop_string )
	{
		if ( prop_string == 0 || strlen(prop_string) == 0 )
			return;

		std::string props( prop_string );
		tokenizer< char_separator<char> > tok(props, char_separator<char>(";") );
		for ( tokenizer< char_separator<char> >::iterator iter = tok.begin(); iter != tok.end(); ++iter )
		{
			size_t pos = iter->find_first_of('=');
			if (pos != std::string::npos)
				AddChild( trim_copy( iter->substr(0, pos) ) )->m_Value = StringValue( trim_copy( iter->substr(pos + 1) ) );
		}
	}

	PropNode::PropNode( const PropNode& other )
	{
		*this = other;
	}

	PropNode& PropNode::operator=( const PropNode& other )
	{
		m_Value = other.m_Value;
		m_Children.clear();
		for ( ConstChildIter iter = other.m_Children.begin(); iter != other.m_Children.end(); ++iter )
			m_Children.push_back( std::make_pair( iter->first, PropNodePtr( new PropNode( *iter->second ) ) ) );
		return *this;
	}

	PropNode& PropNode::Merge( const PropNode& other, bool overwrite )
	{
		if ( overwrite )
			m_Value = other.m_Value;

		for ( ConstChildIter other_it = other.m_Children.begin(); other_it != other.m_Children.end(); ++other_it )
		{
			// see if we already have the key
			PropNodePtr child = nullptr;
			for ( ChildContainer::iterator this_it = m_Children.begin(); this_it != m_Children.end(); ++this_it )
			{
				if ( this_it->first == other_it->first )
					child = this_it->second;
			}

			if ( child == nullptr )
				m_Children.push_back( std::make_pair( other_it->first, PropNodePtr( new PropNode( *other_it->second ) ) ) );
			else child->Merge( *other_it->second, overwrite );
		}

		return *this;
	}

	PropNode::~PropNode()
	{
	}

	PropNodePtr PropNode::AddChild( const String& key )
	{
		size_t ofs = key.find_first_of( '.' );
		if ( ofs == String::npos )
		{
			// create first-level child
			m_Children.push_back( std::make_pair( key, PropNodePtr( new PropNode ) ) );
			return m_Children.back().second;
		}
		else
		{
			// create sub-children if necessary
			String head_key = key.substr( 0, ofs );
			String tail_key = key.substr( ofs + 1 );
			PropNodePtr head_child = GetChildPtr( head_key );
			if ( head_child == nullptr )
				head_child = AddChild( head_key );

			return head_child->AddChild( tail_key );
		}
	}

	const PropNodePtr PropNode::GetChildPtr( const String& key ) const
	{
		size_t ofs = key.find_first_of( '.' );
		if ( ofs == String::npos )
		{
			// find first-level child
			for ( ConstChildIter iter = m_Children.begin(); iter != m_Children.end(); ++iter )
			{
				if ( iter->first == key )
					return iter->second;
			}
			return nullptr;
		}
		else
		{
			// search children
			String head_key = key.substr( 0, ofs );
			String tail_key = key.substr( ofs + 1 );

			for ( ConstChildIter iter = m_Children.begin(); iter != m_Children.end(); ++iter )
			{
				if ( iter->first == head_key )
				{
					PropNodePtr p = iter->second->GetChildPtr( tail_key );
					if ( p != nullptr )
						return p; // child has been found
				}
			}
			return nullptr;
		}
	}

	void PropNode::RemoveChild( const String& key )
	{
		size_t ofs = key.find_first_of( '.' );
		if ( ofs == String::npos )
		{
			// remove first matching child
			for ( ChildIter child_iter = m_Children.begin(); child_iter != m_Children.end(); ++child_iter )
			{
				if ( child_iter->first == key )
				{
					m_Children.erase( child_iter );
					return;
				}
			}
		}
		else
		{
			// search sub keys
			String head_key = key.substr( 0, ofs );
			String tail_key = key.substr( ofs + 1 );
			PropNodePtr head_child = GetChildPtr( head_key );
			head_child->RemoveChild( tail_key );
			if ( head_child->IsEmpty() )
				RemoveChild( head_key );
		}
	}

	void PropNode::RemoveChildren( const String& key )
	{
		size_t ofs = key.find_first_of( '.' );
		if ( ofs == String::npos )
		{
			// remove all children
			ChildIter child_iter = m_Children.begin();
			while ( child_iter != m_Children.end() )
			{
				if ( child_iter->first == key )
					child_iter = m_Children.erase( child_iter );
				else ++child_iter;
			}
		}
		else
		{
			// search sub keys
			String head_key = key.substr( 0, ofs );
			String tail_key = key.substr( ofs + 1 );
			PropNodePtr head_child = GetChildPtr( head_key );
			head_child->RemoveChildren( tail_key );
			if ( head_child->IsEmpty() )
				RemoveChild( head_key );
		}
	}

	void ToPropertyTree( ptree& tree, const PropNode& props, const String& key )
	{
		ptree& child = key.empty() ? tree : tree.add( key, props.GetValue().GetData() );
		for ( PropNode::ConstChildIter iter = props.Begin(); iter != props.End(); ++iter )
			ToPropertyTree( child, *iter->second, iter->first );
	}

	void FromPropertyTree( PropNode& props, const ptree& tree )
	{
		props.Set( tree.get_value("") );
		BOOST_FOREACH( const ptree::value_type &v, tree )
		{
			PropNodePtr child = props.AddChild( v.first );
			FromPropertyTree( *child, v.second );
		}
	}

	void PropNode::ToXmlFile( const String& filename, const String& rootname )
	{
		ptree pt;
		ToPropertyTree( pt, *this, rootname );
		boost::property_tree::xml_writer_settings<char> settings('\t', 1);
		write_xml( filename, pt, std::locale(), settings );
	}

	void PropNode::FromXmlFile( const String& filename, const String& rootname )
	{
		ptree pt;
		read_xml( filename, pt, boost::property_tree::xml_parser::trim_whitespace );
		FromPropertyTree( *this, rootname.empty() ? pt : pt.get_child( rootname ) );
	}

	void PropNode::ToIniFile( const String& filename )
	{
		ptree pt;
		ToPropertyTree( pt, *this, m_Value );
		write_ini( filename, pt );
	}

	void PropNode::FromIniFile( const String& filename )
	{
		ptree pt;
		read_ini( filename, pt );
		FromPropertyTree( *this, pt );
	}

	void PropNode::ToInfoFile( const String& filename )
	{
		ptree pt;
		ToPropertyTree( pt, *this, m_Value );
		write_info( filename, pt );
	}

	void PropNode::FromInfoFile( const String& filename )
	{
		ptree pt;
		read_info( filename, pt );
		FromPropertyTree( *this, pt );
	}

	void PropNode::ToStream( std::ostream& str, const std::string& prefix ) const
	{
		for ( ConstChildIter iter = m_Children.begin(); iter != m_Children.end(); ++iter )
		{
			str << prefix << iter->first;
			if ( iter->second->HasValue() )
				str << " = " << iter->second->GetValue().GetData();
			str << std::endl;

			iter->second->ToStream( str, prefix + "  " );
		}
	}
}