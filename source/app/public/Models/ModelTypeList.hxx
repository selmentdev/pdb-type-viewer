#pragma once
#include <QAbstractItemModel>
#include <LibPtv/Session.hxx>
#include <LibPtv/Type.hxx>
#include <optional>

namespace ptvapp::models
{
    class TypeListElement
    {
    private:
        const LibPdb::Type* m_Type;

        std::optional<uint64_t> m_Padding;

    public:
        explicit TypeListElement(
            const LibPdb::Type* type
        ) noexcept
            : m_Type{ type }
        {
        }

    public:
        const LibPdb::Type* GetType() const noexcept
        {
            return this->m_Type;
        }

        const std::optional<uint64_t>& GetPadding() const noexcept
        {
            return this->m_Padding;
        }
    };
}

namespace ptvapp::models
{
    class TypeListModel : public QAbstractItemModel
    {
        Q_OBJECT
    private:
        QList<TypeListElement> m_Types;

    public:
        explicit TypeListModel(
            QObject* parent = nullptr
        ) noexcept;

        virtual ~TypeListModel() noexcept;

        void Setup(
            const std::vector<std::unique_ptr<LibPdb::Type>>& types
        ) noexcept;

    public:
        virtual int rowCount(
            const QModelIndex& parent = {}
        ) const override;

        virtual Qt::ItemFlags flags(
            const QModelIndex& index
        ) const override;

        virtual QVariant headerData(
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole
        ) const override;

        virtual QVariant data(
            const QModelIndex& index,
            int role = Qt::DisplayRole
        ) const override;

        virtual QModelIndex index(
            int row,
            int column,
            const QModelIndex& parent = {}
        ) const override;

        virtual QModelIndex parent(
            const QModelIndex& child
        ) const override;

        virtual int columnCount(
            const QModelIndex& parent = {}
        ) const override;
    };
}
