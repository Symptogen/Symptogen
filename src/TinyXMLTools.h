#ifndef _H_SYMPTOGEN_PARSER_TINYXMLTOOLS_H_
#define _H_SYMPTOGEN_PARSER_TINYXMLTOOLS_H_

#include <cstdlib>
#include "tinyxml2.h"

const char * getIndent( unsigned int numIndents )
{
    static const char * pINDENT = "                                      + ";
    static const unsigned int LENGTH = strlen( pINDENT );

    if ( numIndents > LENGTH ) numIndents = LENGTH;

    return &pINDENT[ LENGTH-numIndents ];
}

void dump_to_stdout( tinyxml2::XMLNode* pParent, unsigned int indent = 0 )
{
    if ( !pParent ) return;

	tinyxml2::XMLText *pText;
    int t = pParent->Type();
    printf( "%s", getIndent( indent));

    switch ( t )
    {
    case TiXmlNode::DOCUMENT:
        printf( "Document" );
        break;

    case TiXmlNode::ELEMENT:
        printf( "Element \"%s\"", pParent->Value() );
        break;

    case TiXmlNode::COMMENT:
        printf( "Comment: \"%s\"", pParent->Value());
        break;

    case TiXmlNode::UNKNOWN:
        printf( "Unknown" );
        break;

    case TiXmlNode::TEXT:
        pText = pParent->ToText();
        printf( "Text: [%s]", pText->Value() );
        break;

    case TiXmlNode::DECLARATION:
        printf( "Declaration" );
        break;
    default:
        break;
    }
    printf( "\n" );

    TiXmlNode * pChild;

    for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        dump_to_stdout( pChild, indent+2 );
    }
}

#endif