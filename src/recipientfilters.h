#ifndef RECIPIENT_FILTERS_H
#define RECIPIENT_FILTERS_H

#include <irecipientfilter.h>

class ChatFilter : public IRecipientFilter
{
    private:
        int recipient;
    public:
        ChatFilter(int rec);
        virtual bool    IsReliable( void ) const;
        virtual bool    IsInitMessage( void ) const;
        virtual int     GetRecipientCount( void ) const;
        virtual int     GetRecipientIndex( int slot ) const;
};

class RecipientFilter : public IRecipientFilter {
    private:
        bool reliable;
        int *pRecipients;
        int nRecipients;
        int maxRecipients;
    public:
        RecipientFilter(int maxRecipients);
        virtual ~RecipientFilter();
        void MakeReliable();
        void AddRecipient(int recipient);
        virtual bool    IsReliable() const;
        virtual bool    IsInitMessage() const;
        virtual int     GetRecipientCount() const;
        virtual int     GetRecipientIndex(int slot) const;
};

#endif
