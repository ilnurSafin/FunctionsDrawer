#ifndef INDEXWITHWHATSINDEXED_H
#define INDEXWITHWHATSINDEXED_H


class IndexWithWhatsIndexed
{
public:
    IndexWithWhatsIndexed(int idx,int whatsIndexed=0)
        : index(idx),whatIsIndexed(whatsIndexed)
    {
    }
    IndexWithWhatsIndexed()
        :IndexWithWhatsIndexed(0)
    {

    }
    operator int()const
    {
        return index;
    }
    IndexWithWhatsIndexed& operator =(int idx)
    {
        index=idx;
        return *this;
    }
    IndexWithWhatsIndexed& operator =(const IndexWithWhatsIndexed&) = default;
    ~IndexWithWhatsIndexed()
    {
    }


    int index;
    int whatIsIndexed;
};

#endif // INDEXWITHWHATSINDEXED_H
