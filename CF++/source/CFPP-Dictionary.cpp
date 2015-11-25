/*******************************************************************************
 * Copyright (c) 2014, Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * Distributed under the Boost Software License, Version 1.0.
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

/*!
 * @file        CFPP-Dictionary.cpp
 * @copyright   (c) 2014 - Jean-David Gadina - www.xs-labs.com / www.digidna.net
 * @abstract    CoreFoundation++ CFDictionaryRef wrapper
 */

#include <CF++.hpp>

static bool __hasCallBacks = false;

static CFDictionaryKeyCallBacks   __keyCallbacks;
static CFDictionaryValueCallBacks __valueCallbacks;

#ifdef _WIN32

static const void *	__CFDictionaryRetainCallBack( CFAllocatorRef allocator, const void * value );
static const void *	__CFDictionaryRetainCallBack( CFAllocatorRef allocator, const void * value )
{
    ( void )allocator;
    
    if( value != NULL )
    {
        value = CFRetain( value );
    }
    
    return value;
}

static void __CFDictionaryReleaseCallBack( CFAllocatorRef allocator, const void * value );
static void __CFDictionaryReleaseCallBack( CFAllocatorRef allocator, const void * value )
{
    ( void )allocator;
    
    if( value != NULL )
    {
        CFRelease( value );
    }
}

static CFStringRef __CFDictionaryCopyDescriptionCallBack( const void * value );
static CFStringRef __CFDictionaryCopyDescriptionCallBack( const void * value )
{
    if( value == NULL )
    {
        return CFStringCreateWithCString( ( CFAllocatorRef )NULL, "(null)", kCFStringEncodingUTF8 );
    }
    
    return CFCopyDescription( value );
}

static Boolean __CFDictionaryEqualCallBack( const void * value1, const void * value2 );
static Boolean __CFDictionaryEqualCallBack( const void * value1, const void * value2 )
{
    if( value1 == value2 )
    {
        return true;
    }
    
    if( value1 == NULL || value2 == NULL )
    {
        return false;
    }
    
    return CFEqual( value1, value2 );
}

static CFHashCode __CFDictionaryHashCallBack( const void * value );
static CFHashCode __CFDictionaryHashCallBack( const void * value )
{
    if( value == NULL )
    {
        return 0;
    }
    
    return CFHash( value );
}

static void __createCallbacks( void )
{
    if( __hasCallBacks == true  )
    {
        return;
    }
    
    __hasCallBacks = true;
    
    __keyCallbacks.version         = 0;
    __keyCallbacks.retain          = __CFDictionaryRetainCallBack;
    __keyCallbacks.release         = __CFDictionaryReleaseCallBack;
    __keyCallbacks.copyDescription = __CFDictionaryCopyDescriptionCallBack;
    __keyCallbacks.equal           = __CFDictionaryEqualCallBack;
    __keyCallbacks.hash            = __CFDictionaryHashCallBack;
    
    __valueCallbacks.version         = 0;
    __valueCallbacks.retain          = __CFDictionaryRetainCallBack;
    __valueCallbacks.release         = __CFDictionaryReleaseCallBack;
    __valueCallbacks.copyDescription = __CFDictionaryCopyDescriptionCallBack;
    __valueCallbacks.equal           = __CFDictionaryEqualCallBack;
}

#else

static void __createCallbacks( void )
{
    if( __hasCallBacks == true  )
    {
        return;
    }
    
    __hasCallBacks      = true;
    __keyCallbacks      = kCFTypeDictionaryKeyCallBacks;
    __valueCallbacks    = kCFTypeDictionaryValueCallBacks;
}

#endif

namespace CF
{
    Dictionary::Dictionary( void ): _cfObject( NULL )
    {
        __createCallbacks();
        
        this->_cfObject = CFDictionaryCreateMutable
        (
            static_cast< CFAllocatorRef >( NULL ),
            0,
            &__keyCallbacks,
            &__valueCallbacks
        );
    }
    
    Dictionary::Dictionary( CFIndex capacity ): _cfObject( NULL )
    {
        __createCallbacks();
        
        this->_cfObject = CFDictionaryCreateMutable
        (
            static_cast< CFAllocatorRef >( NULL ),
            capacity,
            &__keyCallbacks,
            &__valueCallbacks
        );
    }
    
    Dictionary::Dictionary( const Dictionary & value ): _cfObject( NULL )
    {
        __createCallbacks();
        
        if( value._cfObject != NULL )
        {
            this->_cfObject = CFDictionaryCreateMutableCopy
            (
                static_cast< CFAllocatorRef >( NULL ),
                CFDictionaryGetCount( value._cfObject ),
                value._cfObject
            );
        }
    }
    
    Dictionary::Dictionary( const AutoPointer & value ): _cfObject( NULL )
    {
        __createCallbacks();
        
        if( value.IsValid() && value.GetTypeID() == this->GetTypeID() )
        {
            this->_cfObject = CFDictionaryCreateMutableCopy
            (
                static_cast< CFAllocatorRef >( NULL ),
                CFDictionaryGetCount( value ),
                value
            );
        }
    }
    
    Dictionary::Dictionary( CFTypeRef cfObject ): _cfObject( NULL )
    {
        __createCallbacks();
        
        if( cfObject != NULL && CFGetTypeID( cfObject ) == this->GetTypeID() )
        {
            this->_cfObject = CFDictionaryCreateMutableCopy
            (
                static_cast< CFAllocatorRef >( NULL ),
                CFDictionaryGetCount( static_cast< CFDictionaryRef >( cfObject ) ),
                static_cast< CFDictionaryRef >( cfObject )
            );
        }
    }
    
    Dictionary::Dictionary( CFDictionaryRef cfObject ): _cfObject( NULL )
    {
        __createCallbacks();
        
        if( cfObject != NULL && CFGetTypeID( cfObject ) == this->GetTypeID() )
        {
            this->_cfObject = CFDictionaryCreateMutableCopy
            (
                static_cast< CFAllocatorRef >( NULL ),
                CFDictionaryGetCount( cfObject ),
                cfObject
            );
        }
    }
    
    #ifdef CFPP_HAS_CPP11
    
    Dictionary::Dictionary( std::initializer_list< Pair > value ): Dictionary( static_cast< CFIndex >( value.size() ) )
    {
        for( Pair p: value )
        {
            if( p.GetKey() == nullptr || p.GetValue() == nullptr )
            {
                continue;
            }
            
            CFDictionarySetValue( this->_cfObject, p.GetKey(), p.GetValue() );
        }
    }
    
    Dictionary::Dictionary( Dictionary && value )
    {
        this->_cfObject = value._cfObject;
        value._cfObject = nullptr;
    }
    
    #endif
    
    Dictionary::~Dictionary( void )
    {
        if( this->_cfObject != NULL )
        {
            CFRelease( this->_cfObject );
            
            this->_cfObject = NULL;
        }
    }
    
    Dictionary & Dictionary::operator = ( Dictionary value )
    {
        swap( *( this ), value );
        
        return *( this );
    }
    
    Dictionary & Dictionary::operator = ( const AutoPointer & value )
    {
        return operator =( Dictionary( value ) );
    }
    
    Dictionary & Dictionary::operator = ( CFTypeRef value )
    {
        return operator =( Dictionary( value ) );
    }
    
    Dictionary & Dictionary::operator = ( CFDictionaryRef value )
    {
        return operator =( Dictionary( value ) );
    }
            
    Dictionary & Dictionary::operator += ( Pair pair )
    {
        this->AddValue( pair.GetKey(), pair.GetValue() );
        
        return *( this );
    }
    
    Dictionary & Dictionary::operator << ( Pair pair )
    {
        this->SetValue( pair.GetKey(), pair.GetValue() );
        
        return *( this );
    }
    
    CFTypeRef Dictionary::operator [] ( CFTypeRef key ) const
    {
        return this->GetValue( key );
    }
    
    CFTypeRef Dictionary::operator [] ( const char * key ) const
    {
        return operator []( String( key ) );
    }
    
    CFTypeRef Dictionary::operator [] ( String key ) const
    {
        return this->GetValue( key );
    }
    
    CFTypeID Dictionary::GetTypeID( void ) const
    {
        return CFDictionaryGetTypeID();
    }
    
    CFTypeRef Dictionary::GetCFObject( void ) const
    {
        return this->_cfObject;
    }
    
    bool Dictionary::ContainsKey( CFTypeRef key ) const
    {
        if( this->_cfObject == NULL || key == NULL )
        {
            return false;
        }
        
        return ( CFDictionaryContainsKey( this->_cfObject, key ) ) ? true : false;
    }
    
    bool Dictionary::ContainsValue( CFTypeRef value ) const
    {
        if( this->_cfObject == NULL || value == NULL )
        {
            return false;
        }
        
        return ( CFDictionaryContainsValue( this->_cfObject, value ) ) ? true : false;
    }
    
    void Dictionary::RemoveAllValues( void ) const
    {
        if( this->_cfObject == NULL )
        {
            return;
        }
        
        CFDictionaryRemoveAllValues( this->_cfObject );
    }
    
    CFIndex Dictionary::GetCount( void ) const
    {
        return ( this->_cfObject == NULL ) ? 0 : CFDictionaryGetCount( this->_cfObject );
    }
    
    CFTypeRef Dictionary::GetValue( CFTypeRef key ) const
    {
        if( this->_cfObject == NULL || key == NULL )
        {
            return NULL;
        }
        
        return CFDictionaryGetValue( this->_cfObject, key );
    }
    
    void Dictionary::AddValue( CFTypeRef key, CFTypeRef value ) const
    {
        if( this->_cfObject == NULL || key == NULL || value == NULL )
        {
            return;
        }
        
        return CFDictionaryAddValue( this->_cfObject, key, value );
    }
    
    void Dictionary::RemoveValue( CFTypeRef key ) const
    {
        if( this->_cfObject == NULL || key == NULL )
        {
            return;
        }
        
        return CFDictionaryRemoveValue( this->_cfObject, key );
    }
    
    void Dictionary::ReplaceValue( CFTypeRef key, CFTypeRef value ) const
    {
        if( this->_cfObject == NULL || key == NULL || value == NULL )
        {
            return;
        }
        
        return CFDictionaryReplaceValue( this->_cfObject, key, value );
    }
    
    void Dictionary::SetValue( CFTypeRef key, CFTypeRef value ) const
    {
        if( this->_cfObject == NULL || key == NULL || value == NULL )
        {
            return;
        }
        
        return CFDictionarySetValue( this->_cfObject, key, value );
    }
    
    Dictionary::Iterator Dictionary::begin( void ) const
    {
        return Iterator( this->_cfObject, this->GetCount() );
    }
    
    Dictionary::Iterator Dictionary::end( void ) const
    {
        return Iterator( this->_cfObject, this->GetCount(), this->GetCount() );
    }
    
    void swap( Dictionary & v1, Dictionary & v2 )
    {
        using std::swap;
        
        swap( v1._cfObject, v2._cfObject );
    }
}
