/*
 * cocker - Container Machine Engine
 * author	: calvin
 * email	: calvinwilliams@163.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#include "cocker_in.h"

int DoAction_del_image( struct CockerEnvironment *env )
{
	char		image[ IMAGES_ID_LEN_MAX + 1 ] ;
	char		version[ PATH_MAX + 1 ] ;
	char		*p = NULL ;
	char		*p2 = NULL ;
	char		version_path_base[ PATH_MAX + 1 ] ;
	char		image_path_base[ PATH_MAX + 1 ] ;
	char		image_rlayer_path_base[ PATH_MAX + 1 ] ;
	char		cmd[ 4096 ] ;
	
	int		nret = 0 ;
	
	/* preprocess input parameters */
	if( env->cmd_para.__image )
	{
		memset( image , 0x00 , sizeof(image) );
		strncpy( image , env->cmd_para.__image , sizeof(image)-1 );
	}
	p = image ;
	p2 = strchr( p , ':' ) ;
	memset( version , 0x00 , sizeof(version) );
	if( p2 )
	{
		strncpy( version , p2+1 , sizeof(version)-1 );
		(*p2)= '\0' ;
	}
	if( version[0] == '\0' )
	{
		strcpy( version , "_" );
	}
	
	Snprintf( version_path_base , sizeof(version_path_base)-1 , "%s/%s" , env->images_path_base , image );
	
	Snprintf( image_path_base , sizeof(image_path_base)-1 , "%s/%s/%s" , env->images_path_base , image , version );
	nret = access( image_path_base , F_OK ) ;
	I1TER1( "*** ERROR : image '%s' not found\n" , env->cmd_para.__image )
	
	/* del image folders and files */
	Snprintf( image_rlayer_path_base , sizeof(image_rlayer_path_base)-1 , "%s/rlayer" , image_path_base );
	nret = access( image_rlayer_path_base , F_OK ) ;
	I1TER1( "*** ERROR : image '%s' invalid\n" , env->cmd_para.__image )
	
	nret = SnprintfAndSystem( cmd , sizeof(cmd) , "rm -rf %s" , image_path_base ) ;
	INTER1( "*** ERROR : SnprintfAndSystem [rm -rf %s] failed[%d] , errno[%d]\n" , image_path_base , nret , errno )
	EIDTI( "system [%s] ok\n" , cmd )
	
	if( IsDirectoryEmpty( version_path_base ) == 0 )
	{
		nret = SnprintfAndSystem( cmd , sizeof(cmd) , "rm -rf %s" , version_path_base ) ;
		INTER1( "*** ERROR : SnprintfAndSystem [rm -rf %s] failed[%d] , errno[%d]\n" , version_path_base , nret , errno )
		EIDTI( "system [%s] ok\n" , cmd )
	}
	
	printf( "OK\n" );
	
	return 0;
}

