#include "FinderPattern.h"

namespace zxing {
namespace oned {
namespace rss {

FinderPattern::FinderPattern(int value, std::vector<int> startEnd, int start, int end, int rowNumber)
    : m_value(value),
      m_startEnd(startEnd)
{
    QSharedPointer<std::vector<QSharedPointer<ResultPoint>>> resultPoints(new std::vector<QSharedPointer<ResultPoint>>(2));
    (*resultPoints)[0].reset(new OneDResultPoint(start, rowNumber));
    (*resultPoints)[1].reset(new OneDResultPoint(end, rowNumber));
    m_resultPoints = resultPoints;
}

FinderPattern::FinderPattern(const FinderPattern *other)
{
    m_value = other != nullptr ? other->m_value : 0;
    m_startEnd = other != nullptr ? other->m_startEnd : std::vector<int> ();
    m_resultPoints = other != nullptr ? other->m_resultPoints : QSharedPointer<std::vector<QSharedPointer<ResultPoint>>>();
}

int FinderPattern::getValue() const
{
    return m_value;
}

std::vector<int>& FinderPattern::getStartEnd()
{
    return m_startEnd;
}

QSharedPointer<std::vector<QSharedPointer<ResultPoint>> >& FinderPattern::getResultPoints()
{
    return m_resultPoints;
}

bool FinderPattern::equals(const FinderPattern &other) const
{
    return m_value == other.m_value;
}

int FinderPattern::hashCode() const
{
    return m_value;
}

bool FinderPattern::isValid() const
{
    return m_startEnd.size() != 0;
}

}
}
}
