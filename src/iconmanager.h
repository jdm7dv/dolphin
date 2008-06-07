/***************************************************************************
 *   Copyright (C) 2008 by Peter Penz <peter.penz@gmx.at>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 ***************************************************************************/

#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <kfileitem.h>
#include <kurl.h>

#include <QList>
#include <QObject>
#include <QPixmap>

class DolphinModel;
class DolphinSortFilterProxyModel;
class KJob;
class QAbstractItemView;

/**
 * @brief Manages the icon state of a directory model.
 *
 * Per default a preview is generated for each item.
 * Additionally the clipboard is checked for cut items.
 * The icon state for cut items gets dimmed automatically.
 *
 * The following strategy is used when creating previews:
 * - The previews for currently visible items are created before
 *   the previews for invisible items.
 * - If the user changes the visible area by using the scrollbars,
 *   all pending previews get paused. As soon as the user stays
 *   on the same position for a short delay, the previews are
 *   resumed. Also in this case the previews for the visible items
 *   are generated first.
 */
class IconManager : public QObject
{
    Q_OBJECT

public:
    IconManager(QAbstractItemView* parent, DolphinSortFilterProxyModel* model);
    virtual ~IconManager();
    void setShowPreview(bool show);
    bool showPreview() const;
    void updatePreviews();

private slots:
    /**
     * Generates previews for the items \a items asynchronously.
     */
    void generatePreviews(const KFileItemList &items);

    /**
     * Adds the preview \a pixmap for the item \a item to the preview
     * queue and starts a timer which will dispatch the preview queue
     * later.
     */
    void addToPreviewQueue(const KFileItem& item, const QPixmap& pixmap);

    /**
     * Is invoked when the preview job has been finished and
     * set m_previewJob to 0.
     */
    void slotPreviewJobFinished(KJob* job);

    /** Synchronizes the item icon with the clipboard of cut items. */
    void updateCutItems();

    /**
     * Dispatches the preview queue  block by block within
     * time slices.
     */
    void dispatchPreviewQueue();

    /**
     * Pauses all preview jobs and invokes IconManager::resumePreviews()
     * after a short delay. Is invoked as soon as the user has moved
     * a scrollbar.
     */
    void pausePreviews();

    /**
     * Resumes the previews that have been paused after moving the
     * scrollbar. The previews for the current visible area are
     * generated first.
     */
    void resumePreviews();

private:
    /**
     * Replaces the icon of the item with the \a url by the preview pixmap
     * \a pixmap.
     */
    void replaceIcon(const KUrl& url, const QPixmap& pixmap);

    /**
     * Returns true, if the item \a item has been cut into
     * the clipboard.
     */
    bool isCutItem(const KFileItem& item) const;

    /** Applies an item effect to all cut items. */
    void applyCutItemEffect();

    /**
     * Applies a frame around the icon. False is returned if
     * no frame has been added because the icon is too small.
     */
    bool applyImageFrame(QPixmap& icon);

    /**
     * Resizes the icon to \a maxSize if the icon size does not
     * fit into the maximum size. The aspect ratio of the icon
     * is kept.
     */
    void limitToSize(QPixmap& icon, const QSize& maxSize);

    /**
     * Starts a new preview job for the items \a to m_previewJobs
     * and triggers the preview timer.
     */
    void startPreviewJob(const KFileItemList& items);

    /** Kills all ongoing preview jobs. */
    void killPreviewJobs();

private:
    /** Remembers the pixmap for an item specified by an URL. */
    struct ItemInfo
    {
        KUrl url;
        QPixmap pixmap;
    };

    bool m_showPreview;

    /**
     * True, if m_pendingItems and m_dispatchedItems should be
     * cleared when the preview jobs have been finished.
     */
    bool m_clearItemQueues;

    QAbstractItemView* m_view;
    QTimer* m_previewTimer;
    QTimer* m_scrollAreaTimer;
    QList<KJob*> m_previewJobs;
    DolphinModel* m_dolphinModel;
    DolphinSortFilterProxyModel* m_proxyModel;

    QList<ItemInfo> m_cutItemsCache;
    QList<ItemInfo> m_previews;

    /**
     * Contains the URLs of all items where a preview must be generated, but
     * where the preview job has not dispatched the items yet.
     */
    QList<KUrl> m_pendingItems;

    /**
     * Containts the URLs of all items, where a preview has already been
     * generated by the preview jobs.
     */
    QList<KUrl> m_dispatchedItems;
};

inline bool IconManager::showPreview() const
{
    return m_showPreview;
}

#endif