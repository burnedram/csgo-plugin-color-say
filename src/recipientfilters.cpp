#include "recipientfilters.h"

// ChatFilter impl
ChatFilter::ChatFilter(int rec)
{
    recipient = rec;
}
bool ChatFilter::IsReliable( void ) const
{
    return true;
}
bool ChatFilter::IsInitMessage( void ) const
{
    return false;
}

int ChatFilter::GetRecipientCount( void ) const
{
    return 1;
}
int ChatFilter::GetRecipientIndex( int slot ) const
{
    return  recipient;
}

// RecipientFilter impl
RecipientFilter::RecipientFilter(int maxRecipients) {
    this->reliable = false;
    this->pRecipients = new int[maxRecipients];
    this->nRecipients = 0;
    this->maxRecipients = maxRecipients;
}

RecipientFilter::~RecipientFilter() {
    delete[] pRecipients;
}

// Don't change add any more recipients after you've called this
void RecipientFilter::MakeReliable() {
    reliable = true;
}

void RecipientFilter::AddRecipient(int recipient) {
    if(reliable || nRecipients == maxRecipients)
        return;
    pRecipients[nRecipients++] = recipient;
}

bool RecipientFilter::IsReliable() const {
    return reliable;
}

bool RecipientFilter::IsInitMessage() const {
    return false;
}

int RecipientFilter::GetRecipientCount() const {
    return nRecipients;
}

int RecipientFilter::GetRecipientIndex(int slot) const {
    return pRecipients[slot];
}
