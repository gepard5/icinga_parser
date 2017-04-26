/*
 * =====================================================================================
 *
 *       Filename:  icinga_object.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.04.2017 21:23:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



class IcingaObject {
	public:
		void addKeyValue( const std::string& key, const std::string& value) = 0;
		virtual void addUse( const std::string& value ) = 0;
		virtual void addMember( const std::string& member ) = 0;
	protected:
		std::map< std::string, std::string> properties;
};

class Host : public IcingaObject {
	public:
		void addKeyValue( const std::string& key, const std::string& value);
		void addUse( const std::string& value );
		void addMember( const std::string& member );
}
