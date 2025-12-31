// /*
//  * Copyright (c) 2014 Natale Patriciello <natale.patriciello@gmail.com>
//  *
//  * SPDX-License-Identifier: GPL-2.0-only
//  *
//  */

// #include "tcp-hybla.h"

// #include "tcp-socket-state.h"

// #include "ns3/log.h"

// namespace ns3
// {

// NS_LOG_COMPONENT_DEFINE("TcpHybla");
// NS_OBJECT_ENSURE_REGISTERED(TcpHybla);

// TypeId
// TcpHybla::GetTypeId()
// {
//     static TypeId tid = TypeId("ns3::TcpHybla")
//                             .SetParent<TcpNewReno>()
//                             .AddConstructor<TcpHybla>()
//                             .SetGroupName("Internet")
//                             .AddAttribute("RRTT",
//                                           "Reference RTT",
//                                           TimeValue(MilliSeconds(50)),
//                                           MakeTimeAccessor(&TcpHybla::m_rRtt),
//                                           MakeTimeChecker())
//                             .AddTraceSource("Rho",
//                                             "Rho parameter of Hybla",
//                                             MakeTraceSourceAccessor(&TcpHybla::m_rho),
//                                             "ns3::TracedValueCallback::Double");
//     return tid;
// }

// TcpHybla::TcpHybla()
//     : TcpNewReno(),
//       m_rho(1.0),
//       m_cWndCnt(0)
// {
//     NS_LOG_FUNCTION(this);
// }

// TcpHybla::TcpHybla(const TcpHybla& sock)
//     : TcpNewReno(sock),
//       m_rho(sock.m_rho),
//       m_cWndCnt(sock.m_cWndCnt)
// {
//     NS_LOG_FUNCTION(this);
// }

// TcpHybla::~TcpHybla()
// {
//     NS_LOG_FUNCTION(this);
// }

// void
// TcpHybla::RecalcParam(const Ptr<TcpSocketState>& tcb)
// {
//     NS_LOG_FUNCTION(this);

//     m_rho = std::max((double)tcb->m_minRtt.GetMilliSeconds() / m_rRtt.GetMilliSeconds(), 1.0);

//     NS_ASSERT(m_rho > 0.0);
//     NS_LOG_DEBUG("Calculated rho=" << m_rho);
// }

// void
// TcpHybla::PktsAcked(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt)
// {
//     NS_LOG_FUNCTION(this << tcb << segmentsAcked << rtt);

//     if (rtt == tcb->m_minRtt)
//     {
//         RecalcParam(tcb);
//         NS_LOG_DEBUG("min rtt seen: " << rtt);
//     }
// }

// uint32_t
// TcpHybla::SlowStart(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
// {
//     NS_LOG_FUNCTION(this << tcb << segmentsAcked);

//     NS_ASSERT(tcb->m_cWnd <= tcb->m_ssThresh);

//     if (segmentsAcked >= 1)
//     {
//         /*
//          * slow start
//          * INC = 2^RHO - 1
//          */

//         double increment = std::pow(2, m_rho) - 1.0;
//         auto incr = static_cast<uint32_t>(increment * tcb->m_segmentSize);
//         NS_LOG_INFO("Slow start: inc=" << increment);

//         tcb->m_cWnd = std::min(tcb->m_cWnd + incr, tcb->m_ssThresh);

//         NS_LOG_INFO("In SlowStart, updated to cwnd " << tcb->m_cWnd << " ssthresh "
//                                                      << tcb->m_ssThresh << " with an increment of "
//                                                      << increment * tcb->m_segmentSize);

//         return segmentsAcked - 1;
//     }

//     return 0;
// }

// void
// TcpHybla::CongestionAvoidance(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
// {
//     NS_LOG_FUNCTION(this << tcb << segmentsAcked);

//     uint32_t segCwnd;
//     double increment;

//     while (segmentsAcked > 0)
//     {
//         /*
//          * congestion avoidance
//          * INC = RHO^2 / W
//          */
//         segCwnd = tcb->GetCwndInSegments();
//         increment = std::pow(m_rho, 2) / static_cast<double>(segCwnd);
//         if (tcb->m_congState == TcpSocketState::CA_RECOVERY)
//         {
//             // Use cubic growth model in recovery
//             double cubicFactor = std::pow(segCwnd, 1.5);
//             increment = cubicFactor * m_rho;
//         }
//         else
//         {
//             double beta = 1 + (1 / std::sqrt(segCwnd));
//             increment = beta * (std::pow(m_rho, 2) / static_cast<double>(segCwnd));
//         }
//         m_cWndCnt += increment;
//         segmentsAcked -= 1;
//     }

//     if (m_cWndCnt >= 1.0)
//     {
//         // double to int truncates every time.
//         auto inc = static_cast<uint32_t>(m_cWndCnt);
//         m_cWndCnt -= inc;

//         NS_ASSERT(m_cWndCnt >= 0.0);

//         /* This leaves space for a tcp pacing implementation; it would be easy
//            to setup a limit on the maximum increment of the cWnd per ACK received.
//            The remaining increment is leaved for the next ACK. */

//         tcb->m_cWnd += inc * tcb->m_segmentSize;

//         NS_LOG_INFO("In CongAvoid, updated to cwnd " << tcb->m_cWnd << " ssthresh "
//                                                      << tcb->m_ssThresh << " with an increment of "
//                                                      << inc * tcb->m_segmentSize);
//     }
// }

// Ptr<TcpCongestionOps>
// TcpHybla::Fork()
// {
//     return CopyObject<TcpHybla>(this);
// }

// std::string
// TcpHybla::GetName() const
// {
//     return "TcpHybla";
// }

// } // namespace ns3


/*
 * Copyright (c) 2014 Natale Patriciello <natale.patriciello@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

#include "tcp-hybla.h"

#include "tcp-socket-state.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("TcpHybla");
NS_OBJECT_ENSURE_REGISTERED(TcpHybla);

TypeId
TcpHybla::GetTypeId()
{
    static TypeId tid = TypeId("ns3::TcpHybla")
                            .SetParent<TcpNewReno>()
                            .AddConstructor<TcpHybla>()
                            .SetGroupName("Internet")
                            .AddAttribute("RRTT",
                                          "Reference RTT",
                                          TimeValue(MilliSeconds(50)),
                                          MakeTimeAccessor(&TcpHybla::m_rRtt),
                                          MakeTimeChecker())
                            .AddAttribute("ChannelCapacity",
                                        "Bottleneck link capacity in bits per second",
                                        UintegerValue(1000000), // 1 Mbps default
                                        MakeUintegerAccessor(&TcpHybla::m_channelCapacity),
                                        MakeUintegerChecker<uint32_t>())
                            .AddTraceSource("Rho",
                                            "Rho parameter of Hybla",
                                            MakeTraceSourceAccessor(&TcpHybla::m_rho),
                                            "ns3::TracedValueCallback::Double");
    return tid;
}

TcpHybla::TcpHybla()
    : TcpNewReno(),
      m_rho(1.0),
      m_rRtt(MilliSeconds(50)),
      m_cWndCnt(0),
      m_bwe(0),
      m_lastBweUpdateTime(Time::Min()),
      m_bytesAcked(0),
      m_channelCapacity(1000000)  // 1 Mbps default
{
    
    NS_LOG_FUNCTION(this);
}

TcpHybla::TcpHybla(const TcpHybla& sock)
    : TcpNewReno(sock),
      m_rho(sock.m_rho),
      m_rRtt(sock.m_rRtt),
      m_cWndCnt(sock.m_cWndCnt),
      m_bwe(sock.m_bwe),
      m_lastBweUpdateTime(sock.m_lastBweUpdateTime),
      m_bytesAcked(sock.m_bytesAcked),
      m_channelCapacity(sock.m_channelCapacity)
{
    NS_LOG_FUNCTION(this);
}

TcpHybla::~TcpHybla()
{
    NS_LOG_FUNCTION(this);
}

void
TcpHybla::RecalcParam(const Ptr<TcpSocketState>& tcb)
{
    NS_LOG_FUNCTION(this);

    m_rho = std::max((double)tcb->m_minRtt.GetMilliSeconds() / m_rRtt.GetMilliSeconds(), 1.0);

    NS_ASSERT(m_rho > 0.0);
    NS_LOG_DEBUG("Calculated rho=" << m_rho);
}

void 
TcpHybla::UpdateBandwidthEstimate(Ptr<TcpSocketState> tcb, uint32_t bytesAcked)
{
    if (m_lastBweUpdateTime == Time::Min())
    {
        // First update
        m_lastBweUpdateTime = tcb->m_lastRtt;
        m_bytesAcked = bytesAcked;
        return;
    }

    Time timeDelta = tcb->m_lastRtt - m_lastBweUpdateTime;

    if (timeDelta.GetSeconds() >= 0.1) // Update BWE every 100ms
    {
        m_bwe = static_cast<double>(m_bytesAcked * 8) / timeDelta.GetSeconds();
        m_bytesAcked = bytesAcked;
        m_lastBweUpdateTime = tcb->m_lastRtt;
        NS_LOG_DEBUG("Updated BWE to " << m_bwe << " bps");
    }
    else
    {
        m_bytesAcked += bytesAcked;
    }
}

uint32_t
TcpHybla::CalculateWindow(Ptr<TcpSocketState> tcb)
{
    uint32_t packetsInFlight = tcb->m_cWnd / tcb->m_segmentSize;
    uint32_t newWindowSize;
    if (m_bwe < m_channelCapacity / 2)
    {
      uint32_t residualCapacity = static_cast<uint32_t>((m_channelCapacity - m_bwe) * tcb->m_minRtt.GetSeconds () / (tcb->m_segmentSize * 8));
      newWindowSize = std::max(residualCapacity, static_cast<uint32_t>(0.75 * packetsInFlight)) * tcb->m_segmentSize;
    }
    else
    {
      uint32_t minimumPipeCapacity = static_cast<uint32_t>(m_bwe * tcb->m_minRtt.GetSeconds () / (tcb->m_segmentSize * 8));
      newWindowSize = std::max(minimumPipeCapacity, static_cast<uint32_t>(0.75 * packetsInFlight)) * tcb->m_segmentSize;
    }

    return newWindowSize;
}

void
TcpHybla::CongestionStateSet(Ptr<TcpSocketState> tcb, const TcpSocketState::TcpCongState_t newState)
{
    NS_LOG_FUNCTION(this << tcb << newState);

    if (newState == TcpSocketState::CA_RECOVERY || newState == TcpSocketState::CA_LOSS)
    {
        uint32_t segSize = tcb->m_segmentSize;
        uint32_t packetsInFlight = tcb->m_cWnd / segSize;
        uint32_t newWindow=CalculateWindow(tcb);
        tcb->m_ssThresh = newWindow;
        
        if (newState == TcpSocketState::CA_LOSS)
        {           
            // Set to a fraction of current window to avoid spikes
            tcb->m_cWnd = std::max(2 * segSize, static_cast<uint32_t>(0.5 * packetsInFlight * segSize));
        }
        else // For triple duplicate ACKs (CA_RECOVERY)
        {
            // Don't let window grow beyond current during recovery
            tcb->m_cWnd = std::min(newWindow, tcb->m_cWnd.Get());
        }
        
        NS_LOG_INFO("Congestion event: State=" << newState << 
                    " cwnd=" << tcb->m_cWnd << 
                    " ssthresh=" << tcb->m_ssThresh);
    }
}

void
TcpHybla::PktsAcked(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked, const Time& rtt)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked << rtt);

    if (rtt == tcb->m_minRtt)
    {
        RecalcParam(tcb);
        NS_LOG_DEBUG("min rtt seen: " << rtt);
    }

    UpdateBandwidthEstimate(tcb, segmentsAcked * tcb->m_segmentSize);
}

uint32_t
TcpHybla::SlowStart(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);

    NS_ASSERT(tcb->m_cWnd <= tcb->m_ssThresh);

    if (segmentsAcked >= 1)
    {
        /*
         * slow start
         * INC = 2^RHO - 1
         */

        double increment = std::pow(2, m_rho) - 1.0;
        auto incr = static_cast<uint32_t>(increment * tcb->m_segmentSize);
        NS_LOG_INFO("Slow start: inc=" << increment);

        tcb->m_cWnd = std::min(tcb->m_cWnd + incr, tcb->m_ssThresh);

        NS_LOG_INFO("In SlowStart, updated to cwnd " << tcb->m_cWnd << " ssthresh "
                                                     << tcb->m_ssThresh << " with an increment of "
                                                     << increment * tcb->m_segmentSize);

        return segmentsAcked - 1;
    }

    return 0;
}

void
TcpHybla::CongestionAvoidance(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
    NS_LOG_FUNCTION(this << tcb << segmentsAcked);

    uint32_t segCwnd;
    double increment;

    while (segmentsAcked > 0)
    {
        /*
         * congestion avoidance
         * INC = RHO^2 / W
         */
        segCwnd = tcb->GetCwndInSegments();

        // increment = std::pow(m_rho, 2) / static_cast<double>(segCwnd);
        if (tcb->m_congState == TcpSocketState::CA_RECOVERY)
        {
            // Use cubic growth model in recovery
            double cubicFactor = std::pow(segCwnd, 1.5);
            increment = cubicFactor * m_rho;
        }
        else
        {
            double beta = 1;
            if(segCwnd!=0) beta += (1 / std::sqrt(segCwnd));
            increment = beta * (std::pow(m_rho, 2) / static_cast<double>(segCwnd));
        }

        m_cWndCnt += increment;
        segmentsAcked -= 1;
    }

    if (m_cWndCnt >= 1.0)
    {
        // double to int truncates every time.
        auto inc = static_cast<uint32_t>(m_cWndCnt);
        m_cWndCnt -= inc;

        NS_ASSERT(m_cWndCnt >= 0.0);

        /* This leaves space for a tcp pacing implementation; it would be easy
           to setup a limit on the maximum increment of the cWnd per ACK received.
           The remaining increment is leaved for the next ACK. */

        tcb->m_cWnd += inc * tcb->m_segmentSize;

        NS_LOG_INFO("In CongAvoid, updated to cwnd " << tcb->m_cWnd << " ssthresh "
                                                     << tcb->m_ssThresh << " with an increment of "
                                                     << inc * tcb->m_segmentSize);
    }
}



Ptr<TcpCongestionOps>
TcpHybla::Fork()
{
    return CopyObject<TcpHybla>(this);
}

std::string
TcpHybla::GetName() const
{
    return "TcpHybla";
}

} // namespace ns3
