#ifndef FIELDDELETE_HPP
#define FIELDDELETE_HPP

#include "AField.hpp"
#include "RequestInProgress.hpp"
class AMethod;
class FieldDelete : public AField
{
    private :
        AMethod *createGetMethod();
        AMethod *createErrorMethod(Config config, Location location);
        void     delFile();

    public : 
        FieldDelete(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd);
        ~FieldDelete();

        void        fillField();
        void        verifyMissingData();
        void        verifyData();
		void		verifyRedirect(Location const &);
        
        //void        verifyAI(Location const &);
        bool        isAIPath(std::string path, Location const &location);
         
        AMethod *	getAMethod();
};

#endif
